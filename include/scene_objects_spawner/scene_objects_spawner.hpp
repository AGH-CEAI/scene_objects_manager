#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/type_adapter.hpp"
#include "std_msgs/msg/string.hpp"

class SceneObjectsSpawner : public rclcpp::Node {
public:
  SceneObjectsSpawner() : Node("SceneObjectsSpawner"){};
};
