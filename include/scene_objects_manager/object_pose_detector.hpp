#ifndef OBJECT_POSE_DETECTOR_NODE_HPP_INCLUDED
#define OBJECT_POSE_DETECTOR_NODE_HPP_INCLUDED

#include <mutex>
#include <optional>

#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/pose_array.hpp"

#include "scene_objects_manager/srv/detect_blocks_poses.hpp"


namespace sobjmanager {

class ObjectPoseDetectorNode : public rclcpp::Node
{
public:
    ObjectPoseDetectorNode();

private:
    void imageCb(const sensor_msgs::msg::Image::SharedPtr msg);

    void onDetect(
        const std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Request> req,
        std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Response> res);

    std::mutex mtx_;
    std::optional<cv::Mat> last_bgr_;
    rclcpp::Time last_stamp_;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    rclcpp::Service<scene_objects_manager::srv::DetectBlocksPoses>::SharedPtr srv_;

    std::string image_topic_;
    std::string output_frame_;
};

} // namespace sobjmanager
#endif // OBJECT_POSE_DETECTOR_NODE_HPP_INCLUDED
