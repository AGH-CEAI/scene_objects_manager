#include "scene_objects_manager/object_pose_detector.hpp"

namespace sobjmanager {

ObjectPoseDetectorNode::ObjectPoseDetectorNode() : rclcpp::Node("object_pose_detector") {
  aruco_size_ = this->declare_parameter<double>("aruco_size", 0.02);
  image_topic_ = this->declare_parameter<std::string>("image_topic", "/cam_scene/rgb/image_raw");
  cam_info_topic_ = this->declare_parameter<std::string>("cam_info_topic", "/cam_scene/rgb/camera_info");
  output_frame_ = this->declare_parameter<std::string>("output_frame", "base_link");
  cam_frame_ = this->declare_parameter<std::string>("cam_frame", "cam_scene_rgb_camera_optical_frame_cal");

  camera_info_received_ = false;
  z_offset_of_block_ = 0.095;

  tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      image_topic_, rclcpp::SensorDataQoS(), std::bind(&ObjectPoseDetectorNode::image_cb, this, std::placeholders::_1));

  cam_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
      cam_info_topic_, rclcpp::SensorDataQoS(),
      std::bind(&ObjectPoseDetectorNode::camera_info_cb, this, std::placeholders::_1));

  detect_blocks_srv_ = this->create_service<scene_objects_manager::srv::DetectBlocksPoses>(
      "detect_blocks_poses",
      std::bind(&ObjectPoseDetectorNode::on_detect, this, std::placeholders::_1, std::placeholders::_2));

  RCLCPP_INFO(this->get_logger(), "Detector node started. Subscriptionto: %s", image_topic_.c_str());
  RCLCPP_INFO(this->get_logger(), "Service ready: /detect_blocks_poses");
}

void ObjectPoseDetectorNode::image_cb(const sensor_msgs::msg::Image::SharedPtr msg) {
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

void ObjectPoseDetectorNode::on_detect(const std::shared_ptr<DetectBlocksPosesSrv::Request>,
                                       std::shared_ptr<DetectBlocksPosesSrv::Response> res) {
  if (!camera_info_received_) {
    RCLCPP_INFO(this->get_logger(), "Camera info not received yet.");
    return;
  }

  cv::Mat img;
  if (last_rgb_) {
    std::lock_guard<std::mutex> lock(mtx_);
    img = last_rgb_.value().clone();
  }

  if (img.empty()) {
    RCLCPP_ERROR(this->get_logger(), "Failed: no image in buffer to detect objects.");
    return;
  }

  res->poses.header.stamp = this->now();
  res->poses.header.frame_id = output_frame_;
  res->poses.poses.clear();

  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;
  cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  cv::aruco::detectMarkers(img, dictionary, marker_corners, marker_ids, parameters, rejected_candidates);

  cv::aruco::estimatePoseSingleMarkers(marker_corners, aruco_size_, camera_matrix_, dist_coeffs_, rvecs_, tvecs_);

  cv::Mat R;
  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  double k_angle_step = M_PI / 2.0;
  tf2::Quaternion q_yaw_only;

  for (size_t i = 0; i < marker_ids.size(); i++) {
    geometry_msgs::msg::PoseStamped pose_cam_;
    pose_cam_.header.stamp = this->now();
    pose_cam_.header.frame_id = cam_frame_;
    pose_cam_.pose.position.x = tvecs_[i][0];
    pose_cam_.pose.position.y = tvecs_[i][1];
    pose_cam_.pose.position.z = tvecs_[i][2];
    cv::Rodrigues(rvecs_[i], R);
    tf2::Matrix3x3 tf3d(R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), R.at<double>(1, 0),
                        R.at<double>(1, 1), R.at<double>(1, 2), R.at<double>(2, 0), R.at<double>(2, 1),
                        R.at<double>(2, 2));

    tf3d.getRPY(roll, pitch, yaw);

    yaw -= k_angle_step * std::round(yaw / k_angle_step) - k_angle_step;

    q_yaw_only.setRPY(0.0, 0.0, yaw);
    q_yaw_only.normalize();
    pose_cam_.pose.orientation = tf2::toMsg(q_yaw_only);

    geometry_msgs::msg::PoseStamped pose_target;
    try {
      pose_target = tf_buffer_->transform(pose_cam_, output_frame_, tf2::durationFromSec(0.1));
    } catch (const tf2::TransformException& ex) {
      RCLCPP_WARN(this->get_logger(), "TF transforms failed: %s", ex.what());
      continue;
    }

    pose_target.pose.position.z = z_offset_of_block_;

    res->poses.poses.push_back(pose_target.pose);
  }
}

void ObjectPoseDetectorNode::camera_info_cb(const sensor_msgs::msg::CameraInfo::SharedPtr msg) {
  if (camera_info_received_)
    return;

  camera_matrix_ = cv::Mat(3, 3, CV_64F);
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      camera_matrix_.at<double>(row, col) = msg->k[row * 3 + col];
    }
  }

  dist_coeffs_ = cv::Mat(1, static_cast<int>(msg->d.size()), CV_64F);
  for (size_t i = 0; i < msg->d.size(); ++i) {
    dist_coeffs_.at<double>(0, static_cast<int>(i)) = msg->d[i];
  }

  camera_info_received_ = true;

  cam_info_sub_.reset();
}

}  // namespace sobjmanager
