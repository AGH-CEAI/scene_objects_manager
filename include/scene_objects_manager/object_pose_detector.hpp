#ifndef SCENE_OBJECTS_MANAGER__OBJECT_POSE_DETECTOR_HPP_
#define SCENE_OBJECTS_MANAGER__OBJECT_POSE_DETECTOR_HPP_

#include <cmath>
#include <fstream>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <vector>

#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <yaml-cpp/yaml.h>

#include <cv_bridge/cv_bridge.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <rclcpp/rclcpp.hpp>

#include <geometry_msgs/msg/pose_array.hpp>
#include <scene_objects_manager/srv/detect_blocks_poses.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

using DetectBlocksPosesSrv = scene_objects_manager::srv::DetectBlocksPoses;

namespace sobjmanager {

class ObjectPoseDetectorNode : public rclcpp::Node {
 public:
  ObjectPoseDetectorNode();

 private:
  void image_cb(const sensor_msgs::msg::Image::SharedPtr msg);

  void on_detect(const std::shared_ptr<DetectBlocksPosesSrv::Request>,
                 std::shared_ptr<DetectBlocksPosesSrv::Response> res);

  void camera_info_cb(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
  rclcpp::Service<scene_objects_manager::srv::DetectBlocksPoses>::SharedPtr detect_blocks_srv_;
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr cam_info_sub_;

  std::mutex mtx_;
  std::optional<cv::Mat> last_rgb_;
  rclcpp::Time last_rgb_stamp_;
  bool camera_info_received_;

  std::string image_topic_;
  std::string cam_info_topic_;
  std::string cam_frame_;
  std::string output_frame_;
  double aruco_size_;

  cv::Mat camera_matrix_;
  cv::Mat dist_coeffs_;

  std::vector<cv::Vec3d> rvecs, tvecs;

  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
};

}  // namespace sobjmanager
#endif  // SCENE_OBJECTS_MANAGER__OBJECT_POSE_DETECTOR_HPP_
