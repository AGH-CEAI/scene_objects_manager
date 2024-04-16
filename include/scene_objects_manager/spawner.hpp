#ifndef OBJECT_SPAWNER_HPP_INCLUDED
#define OBJECT_SPAWNER_HPP_INCLUDED
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <string>

#include "scene_objects_manager/scene_object.hpp"

using CollisionObject = moveit_msgs::msg::CollisionObject;

namespace sobjspawner {

class Spawner {
public:
  Spawner(std::string planning_frame_id);

  void spawn_object(const SceneObject& obj);

private:
  static CollisionObject make_collision_object(const SceneObject& raw_obj);
  void spawn(const CollisionObject& object);

  std::string _planning_frame_id;
  moveit::planning_interface::PlanningSceneInterface _planning_scene_interface;
};

}  // namespace sobjspawner
#endif  // OBJECT_SPAWNER_HPP_INCLUDED
