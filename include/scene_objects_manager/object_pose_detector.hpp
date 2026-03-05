#ifndef SCENE_OBJECTS_MANAGER__OBJECT_POSE_DETECTOR_HPP_
#define SCENE_OBJECTS_MANAGER__OBJECT_POSE_DETECTOR_HPP_

#include <cv_bridge/cv_bridge.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <yaml-cpp/yaml.h>

#include <mutex>
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <optional>
#include <stdexcept>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <vector>

#include "ament_index_cpp/get_package_share_directory.hpp"
#include "geometry_msgs/msg/pose_array.hpp"
#include "rclcpp/rclcpp.hpp"
#include "scene_objects_manager/srv/detect_blocks_poses.hpp"
#include "sensor_msgs/msg/image.hpp"

namespace sobjmanager {

class ObjectPoseDetectorNode : public rclcpp::Node {
public:
  ObjectPoseDetectorNode();

private:
  void imageCb(const sensor_msgs::msg::Image::SharedPtr msg);

  void onDetect(
      const std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Request> req,
      std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Response> res);

  void readCamCalib();

  //   void cameraInfoCb(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
  rclcpp::Service<scene_objects_manager::srv::DetectBlocksPoses>::SharedPtr srv_;
  //   rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr cam_info_sub_;

  std::mutex mtx_;
  std::optional<cv::Mat> last_rgb_;
  rclcpp::Time last_rgb_stamp_;

  //   std::mutex calib_mutex_;
  //   bool have_calib{false};

  std::string image_topic_;
  std::string output_frame_;

  cv::Mat camera_matrix_;
  cv::Mat dist_coeffs_;

  std::vector<cv::Vec3d> rvecs, tvecs;
  std::string camera_info_path_;
  double aruco_size_{ 0.02 };

  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  std::string camera_frame_;
  std::string target_frame_;
};

}  // namespace sobjmanager
#endif  // SCENE_OBJECTS_MANAGER__OBJECT_POSE_DETECTOR_HPP_

// test
