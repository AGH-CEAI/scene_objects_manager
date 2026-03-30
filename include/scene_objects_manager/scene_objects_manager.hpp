#ifndef SCENE_OBJECTS_MANAGER__SCENE_OBJECTS_MANAGER_HPP_
#define SCENE_OBJECTS_MANAGER__SCENE_OBJECTS_MANAGER_HPP_
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/type_adapter.hpp"
#include "std_msgs/msg/string.hpp"

namespace sobjmanager {

class SceneObjectsManager : public rclcpp::Node {
public:
  SceneObjectsManager() : Node("scene_objects_manager") {};
};

}  // namespace sobjmanager
#endif  // SCENE_OBJECTS_MANAGER__SCENE_OBJECTS_MANAGER_HPP_
