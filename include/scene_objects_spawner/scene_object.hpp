#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <string>

using SolidPrimitive = shape_msgs::msg::SolidPrimitive;
using Pose = geometry_msgs::msg::Pose;

struct SceneObject {
  std::string unique_id_id;
  std::string frame_id;
  SolidPrimitive primitive;
  Pose pose;
};
