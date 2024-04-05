#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/type_adapter.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

template <>
struct rclcpp::TypeAdapter<std::string, std_msgs::msg::String> {
  using is_specialized = std::true_type;
  using custom_type = std::string;
  using ros_message_type = std_msgs::msg::String;

  static void convert_to_ros_message(const custom_type& source, ros_message_type& destination) {
    destination.data = source;
  }

  static void convert_to_custom(const ros_message_type& source, custom_type& destination) {
    destination = source.data;
  }
};

class SceneObjectsSpawner : public rclcpp::Node {
  using MyAdaptedType = rclcpp::TypeAdapter<std::string, std_msgs::msg::String>;

public:
  SceneObjectsSpawner();

private:
  void timer_callback();

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<MyAdaptedType>::SharedPtr publisher_;
  size_t count_;
};
