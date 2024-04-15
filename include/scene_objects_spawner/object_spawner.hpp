#pragma once
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <string>

#include "scene_objects_spawner/scene_object.hpp"

using CollisionObject = moveit_msgs::msg::CollisionObject;

class ObjectsSpawner {
public:
  ObjectsSpawner(std::string planning_frame_id);

  void spawn_object(const SceneObject& obj);

private:
  CollisionObject make_collision_object(const SceneObject& raw_obj) const;
  void spawn(const CollisionObject& object);

  std::string _planning_frame_id;
  moveit::planning_interface::PlanningSceneInterface _planning_scene_interface;
};
