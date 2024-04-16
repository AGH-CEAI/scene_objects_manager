#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/type_adapter.hpp"
#include "std_msgs/msg/string.hpp"

class SceneObjectsManager : public rclcpp::Node {
public:
  SceneObjectsManager() : Node("SceneObjectsManager"){};
};
