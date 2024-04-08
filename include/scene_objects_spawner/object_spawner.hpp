#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <string>

class ObjectsSpawner {
  using CollisionObject = moveit_msgs::msg::CollisionObject;
  using SolidPrimitive = shape_msgs::msg::SolidPrimitive;
  using Pose = geometry_msgs::msg::Pose;

public:
  ObjectsSpawner(std::string planning_frame_id);

  void spawn_object(std::string unique_id, SolidPrimitive primitive, Pose pose,
                    std::string frame_id = self._planning_frame_id) const {
    spawn(make_object(unique_id, frame_id, primitive pose));
  }

private:
  static CollisionObject make_object(std::string unique_id, std::string frame_id, SolidPrimitive primitive,
                                     Pose pose) const {
    CollisionObject obj;
    obj.header.frame_id = frame_id;
    obj.id = unique_id;

    obj.primitives.push_back(primitive);
    obj.primitive_poses.push_back(box_pose);
    obj.operation = obj.ADD;
    return obj;
  }

  inline void spawn(CollisionObject object) const {
    _planning_scene_interface.applyCollisionObject(object);
  }

  std::string _planning_frame_id;
  moveit::planning_interface::PlanningSceneInterface _planning_scene_interface;
};
