#include "scene_objects_manager/object_pose_detector.hpp"

namespace sobjmanager {

// static std::vector<double> readDataVector(const YAML::Node& root, const std::string& key) {
//   if (!root[key] || !root[key]["data"]) {
//     throw std::runtime_error("Missing key '" + key + ".data' in calibration YAML");
//   }
//   return root[key]["data"].as<std::vector<double>>();
// }

ObjectPoseDetectorNode::ObjectPoseDetectorNode() : rclcpp::Node("object_pose_detector") {
  // const auto share = ament_index_cpp::get_package_share_directory("aegis_utils");
  // camera_info_path_ = this->declare_parameter<std::string>("camera_info_path", share +
  // "/config/scene_intrinsics.yaml"); // TODO: cancel

  // TODO(#xx): Read data from ROS2 topic /cam_scene/rgb/camera_info, it contains data from befoure the calib
  // readCamCalib();

  aruco_size_ = this->declare_parameter<double>("aruco_size", aruco_size_);
  image_topic_ = this->declare_parameter<std::string>("image_topic", "/cam_scene/rgb/image_raw");
  cam_info_topic_ = this->declare_parameter<std::string>("cam_info_topic", "/cam_scene/rgb/camera_info");
  output_frame_ = this->declare_parameter<std::string>("output_frame", "base_link");
  cam_frame_ = this->declare_parameter<std::string>("cam_frame", "cam_scene_rgb_camera_optical_frame_cal");

  tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      image_topic_,
      rclcpp::SensorDataQoS(),
      std::bind(&ObjectPoseDetectorNode::imageCb, this, std::placeholders::_1));

  cam_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
      cam_info_topic_,
      rclcpp::SensorDataQoS(),
      std::bind(&ObjectPoseDetectorNode::cameraInfoCb, this, std::placeholders::_1));

  detect_blocks_srv_ = this->create_service<scene_objects_manager::srv::DetectBlocksPoses>(
      "detect_blocks_poses",
      std::bind(&ObjectPoseDetectorNode::onDetect, this, std::placeholders::_1, std::placeholders::_2));

  RCLCPP_INFO(this->get_logger(), "Detector node started. Subscriptionto: %s", image_topic_.c_str());
  RCLCPP_INFO(this->get_logger(), "Service ready: /detect_blocks_poses");
}

void ObjectPoseDetectorNode::imageCb(const sensor_msgs::msg::Image::SharedPtr msg) {
  cv_bridge::CvImageConstPtr cv_ptr;
  try {
    cv_ptr = cv_bridge::toCvShare(msg, "bgr8");
  } catch (const cv_bridge::Exception& e) {
    RCLCPP_WARN(this->get_logger(), "cv_bridge exception: %s", e.what());
    return;
  }
  {
    std::lock_guard<std::mutex> lock(mtx_);
    last_rgb_ = cv_ptr->image.clone();
    last_rgb_stamp_ = msg->header.stamp;
  }
}

void ObjectPoseDetectorNode::onDetect(
    const std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Request> req,
    std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Response> res) {
  if (!req->detect) {
    RCLCPP_INFO(this->get_logger(), "start_detection=false -> returning empty PoseArray (no detection).");
    return;
  }
  if (!camera_info_received_) {
    RCLCPP_INFO(this->get_logger(), "Camera info not received yet.");
    return;
  }

  res->poses.header.stamp = this->now();
  res->poses.header.frame_id = output_frame_;
  res->poses.poses.clear();

  // Read from folder - testing
  //   std::string path_image;
  //   path_image = "/home/antrad/ceai_ws/getpos_data/scene_4_blocks.png";
  //   cv::Mat img = cv::imread(path_image, cv::IMREAD_COLOR);
  //   if (img.empty()) {
  //     RCLCPP_ERROR(this->get_logger(), "cv::imread failed");
  //     return;
  //   }

  cv::Mat img;
  {
    std::lock_guard<std::mutex> lock(mtx_);
    img = last_rgb_.value().clone();
  }

  if (img.empty()) {
    RCLCPP_ERROR(this->get_logger(), "cv::imread failed");
    return;
  }

  std::vector<int> markerIds;
  std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
  cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  cv::aruco::detectMarkers(img, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);

  cv::Mat output_image = img.clone();
  cv::aruco::drawDetectedMarkers(output_image, markerCorners, markerIds);

  cv::imshow("output", output_image);
  cv::waitKey(1);
  cv::destroyAllWindows();

  cv::aruco::estimatePoseSingleMarkers(markerCorners, aruco_size_, camera_matrix_, dist_coeffs_, rvecs, tvecs);

  for (size_t i = 0; i < markerIds.size(); i++) {
    geometry_msgs::msg::PoseStamped pose_cam_;
    pose_cam_.header.stamp = this->now();
    pose_cam_.header.frame_id = cam_frame_;
    pose_cam_.pose.position.x = tvecs[i][0];
    pose_cam_.pose.position.y = tvecs[i][1];
    pose_cam_.pose.position.z = tvecs[i][2];
    cv::Mat R;
    cv::Rodrigues(rvecs[i], R);
    tf2::Matrix3x3 tf3d(
        R.at<double>(0, 0),
        R.at<double>(0, 1),
        R.at<double>(0, 2),
        R.at<double>(1, 0),
        R.at<double>(1, 1),
        R.at<double>(1, 2),
        R.at<double>(2, 0),
        R.at<double>(2, 1),
        R.at<double>(2, 2));
    tf2::Quaternion q;
    tf3d.getRotation(q);
    q.normalize();
    pose_cam_.pose.orientation = tf2::toMsg(q);

    geometry_msgs::msg::PoseStamped pose_target;
    try {
      pose_target = tf_buffer_->transform(pose_cam_, output_frame_, tf2::durationFromSec(0.1));
    } catch (const tf2::TransformException& ex) {
      RCLCPP_WARN(this->get_logger(), "TF transforms failed: %s", ex.what());
      continue;
    }
    res->poses.poses.push_back(pose_target.pose);
  }
}

void ObjectPoseDetectorNode::cameraInfoCb(const sensor_msgs::msg::CameraInfo::SharedPtr msg) {
  if (camera_info_received_) {
    return;
  }

  camera_matrix_ = cv::Mat(3, 3, CV_64F);
  for (int r = 0; r < 3; ++r) {
    for (int c = 0; c < 3; ++c) {
      camera_matrix_.at<double>(r, c) = msg->k[r * 3 + c];
    }
  }

  dist_coeffs_ = cv::Mat(1, static_cast<int>(msg->d.size()), CV_64F);
  for (size_t i = 0; i < msg->d.size(); ++i) {
    dist_coeffs_.at<double>(0, static_cast<int>(i)) = msg->d[i];
  }

  camera_frame_ = msg->header.frame_id;
  camera_info_received_ = true;

  RCLCPP_INFO(this->get_logger(), "Received camera info from topic.");

  cam_info_sub_.reset();
}

// void ObjectPoseDetectorNode::readCamCalib() {
//   YAML::Node calib;
//   try {
//     calib = YAML::LoadFile(camera_info_path_);
//   } catch (const std::exception& e) {
//     throw std::runtime_error(
//         std::string("Failed to open/parse camera calib YAML '") + camera_info_path_ + "': " + e.what());
//   }
//   const auto K = readDataVector(calib, "camera_matrix");
//   const auto D = readDataVector(calib, "distortion_coefficients");

//   if (K.size() != 9) {
//     throw std::runtime_error("cmera_matrix.data must contains 9 elements");
//   }
//   if (D.empty()) {
//     throw std::runtime_error("distortion_coefficients.data must not be empty");
//   }

//   camera_matrix_ = cv::Mat(3, 3, CV_64F);
//   for (int r = 0; r < 3; ++r) {
//     for (int c = 0; c < 3; ++c) {
//       camera_matrix_.at<double>(r, c) = K[r * 3 + c];
//     }
//   }

//   dist_coeffs_ = cv::Mat(1, static_cast<int>(D.size()), CV_64F);
//   for (size_t i = 0; i < D.size(); ++i) {
//     dist_coeffs_.at<double>(0, static_cast<int>(i)) = D[i];
//   }
// }

}  // namespace sobjmanager
