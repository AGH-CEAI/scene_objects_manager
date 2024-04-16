#ifndef SCENE_OBJECTS_SPAWNER_HPP_INCLUDED
#define SCENE_OBJECTS_SPAWNER_HPP_INCLUDED
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
  SceneObjectsManager() : Node("SceneObjectsManager"){};
};

}  // namespace sobjmanager
#endif  // SCENE_OBJECTS_SPAWNER_HPP_INCLUDED
