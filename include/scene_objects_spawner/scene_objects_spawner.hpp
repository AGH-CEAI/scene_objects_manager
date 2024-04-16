#ifndef SCENE_OBJECTS_SPAWNER_HPP_INCLUDED
#define SCENE_OBJECTS_SPAWNER_HPP_INCLUDED
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
#endif  // SCENE_OBJECTS_SPAWNER_HPP_INCLUDED
