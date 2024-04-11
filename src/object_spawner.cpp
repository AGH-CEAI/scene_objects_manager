#include "scene_objects_spawner/object_spawner.hpp"

ObjectsSpawner::ObjectsSpawner(std::string planning_frame_id)
  : _planning_frame_id(planning_frame_id)
  , _planning_scene_interface(moveit::planning_interface::PlanningSceneInterface()) {
}

void ObjectsSpawner::spawn_object(SceneObject obj) const {
  spawn(make_collision_object(obj));
}

CollisionObject ObjectsSpawner::make_collision_object(SceneObject raw_obj) const {
  CollisionObject obj;
  obj.header.frame_id = raw_obj.frame_id;
  obj.id = raw_obj.unique_id;

  const SolidPrimitive prim;
  prim.type = raw_obj.type;
  prim.dimensions.resize(_METER_DIMENSIONS);
  prim.dimensions[prim.BOX_X] = raw_obj.scale * raw_obj.size.x;
  prim.dimensions[prim.BOX_Y] = raw_obj.scale * raw_obj.size.y;
  prim.dimensions[prim.BOX_Z] = raw_obj.scale * raw_obj.size.z;

  obj.primitives.push_back(prim);
  obj.primitive_poses.push_back(raw_obj.pose);
  obj.operation = obj.ADD;
  return obj;
}

void ObjectsSpawner::spawn(CollisionObject object) const {
  //TODO consider defining COLOR for collision objs
  // 2nd argument: const std_msgs::msg::ColorRGBA &  	object_color
  _planning_scene_interface.applyCollisionObject(object);
}
