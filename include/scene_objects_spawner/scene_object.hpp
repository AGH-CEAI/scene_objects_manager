#include <geometry_msgs/msg/pose.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <cctype>
#include <string>

#include "yaml-cpp/yaml.h"

using Pose = geometry_msgs::msg::Pose;
using Point = geometry_msgs::msg::Point;

struct SceneObject {
  std::string unique_id_id;
  std::string pretty_name;
  std::string frame_id;
  uint8_t type;
  Point size;
  double scale;
  Pose pose;
};
