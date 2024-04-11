#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <string>

#include "scene_objects_spawner/scene_object.hpp"

class ObjectsSpawner {
  using CollisionObject = moveit_msgs::msg::CollisionObject;

public:
  ObjectsSpawner(std::string planning_frame_id);

  void spawn_object(SceneObject obj) const;

private:
  CollisionObject make_collision_object(SceneObject raw_obj) const;
  void spawn(CollisionObject object) const;

  constexpr int _METER_DIMENSIONS{ 3 };
  std::string _planning_frame_id;
  moveit::planning_interface::PlanningSceneInterface _planning_scene_interface;
};
