#include "scene_objects_manager/object_pose_detector.hpp"

namespace sobjmanager {

ObjectPoseDetectorNode::ObjectPoseDetectorNode()
: rclcpp::Node("object_pose_detector")
{
    image_topic_ = this->declare_parameter<std::string>("image_topic", "/cam_scene/rgb/image_raw");
    output_frame_ = this->declare_parameter<std::string>("output_frame", "camera_frame");

    sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        image_topic_,
        rclcpp::SensorDataQoS(),
        std::bind(&ObjectPoseDetectorNode::imageCb, this, std::placeholders::_1)
    );

    srv_ = this->create_service<scene_objects_manager::srv::DetectBlocksPoses>(
        "detect_blocs_poses",
        std::bind(&ObjectPoseDetectorNode::onDetect, this, std::placeholders::_1, std::placeholders::_2)
    );

    RCLCPP_INFO(this->get_logger(), "Detector node started. Subscriptionto: %s", image_topic_.c_str());
    RCLCPP_INFO(this->get_logger(), "Service ready: /detect_blocs_poses");
}

void ObjectPoseDetectorNode::imageCb(const sensor_msgs::msg::Image::SharedPtr msg)
{
    v_bridge::CvImageConstPtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvShare(msg, "bgr8");
    } catch (const cv_bridge::Exception & e) {
        RCLCPP_WARN(this->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mtx_);
        last_bgr_ = cv_ptr->image.clone();
        last_stamp_ = msg->header.stamp;
    }
    RCLCPP_DEBUG(this->get_logger(), "Stored image %dx%d", cv_ptr->image.cols, cv_ptr->image.rows);
}

void ObjectPoseDetectorNode::onDetect(
    const std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Request>,
    std::shared_ptr<scene_objects_manager::srv::DetectBlocksPoses::Response> res
)
{
    RCLCPP_INFO(this->get_logger(), "make the request");
}

} // namespace sobjmanager
