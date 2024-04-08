#include "scene_objects_spawner/object_spawner.hpp"

ObjectsSpawner::ObjectsSpawner(std::string planning_frame_id)
  : _planning_frame_id(planning_frame_id)
  , _planning_scene_interface(moveit::planning_interface::PlanningSceneInterface()) {
}
