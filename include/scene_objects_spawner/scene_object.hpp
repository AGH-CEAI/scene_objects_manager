#ifndef SCENE_OBJECT_HPP_INCLUDED
#define SCENE_OBJECT_HPP_INCLUDED
#include <geometry_msgs/msg/pose.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <cctype>
#include <string>

#include "scene_objects_spawner/utils.hpp"
#include "yaml-cpp/yaml.h"

namespace sobjspawner {

using Point = geometry_msgs::msg::Point;
using Pose = geometry_msgs::msg::Pose;
using SolidPrimitive = shape_msgs::msg::SolidPrimitive;

struct SceneObject {
  std::string unique_id;
  std::string pretty_name;
  std::string frame_id;
  uint8_t type;
  Point size;
  double scale;
  Pose pose;

  static const inline std::map<std::string, uint8_t> PRIMITIVE_STR_MAP = {
    { "box", SolidPrimitive::BOX },   { "sphere", SolidPrimitive::SPHERE }, { "cylinder", SolidPrimitive::CYLINDER },
    { "cone", SolidPrimitive::CONE }, { "prism", SolidPrimitive::PRISM },
  };
  static const inline std::map<uint8_t, std::string> PRIMITIVE_UINT_MAP = reverse_map(PRIMITIVE_STR_MAP);
};

}  // namespace sobjspawner
#endif  // SCENE_OBJECT_HPP_INCLUDED
