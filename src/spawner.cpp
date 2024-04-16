#include "scene_objects_manager/spawner.hpp"

namespace sobjmanager {

Spawner::Spawner(std::string planning_frame_id)
  : _planning_frame_id(planning_frame_id)
  , _planning_scene_interface(moveit::planning_interface::PlanningSceneInterface()) {
}

void Spawner::spawn_object(const SceneObject& obj) {
  spawn(make_collision_object(obj));
}

CollisionObject Spawner::make_collision_object(const SceneObject& raw_obj) {
  CollisionObject obj;
  obj.header.frame_id = raw_obj.frame_id;
  obj.id = raw_obj.unique_id;

  SolidPrimitive prim;
  prim.type = raw_obj.type;
  prim.dimensions.resize(3);
  prim.dimensions[prim.BOX_X] = raw_obj.scale * raw_obj.size.x;
  prim.dimensions[prim.BOX_Y] = raw_obj.scale * raw_obj.size.y;
  prim.dimensions[prim.BOX_Z] = raw_obj.scale * raw_obj.size.z;

  obj.primitives.push_back(prim);
  obj.primitive_poses.push_back(raw_obj.pose);
  obj.operation = obj.ADD;
  return obj;
}

void Spawner::spawn(const CollisionObject& object) {
  _planning_scene_interface.applyCollisionObject(object);
}

}  // namespace sobjmanager
