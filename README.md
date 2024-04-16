# Scene Objects Spawner

A Node for spawning objects in MoveIt2 environment with ROS 2 API.

---

## Development notes
The node functionalities should be based around [MoveIt2 Docs](https://moveit.picknik.ai/main/doc/tutorials/planning_around_objects/planning_around_objects.html#create-a-collision-object).
- Objects types: `box, sphere, cylinder, cone, mesh_from_file, mesh_from_url`
- Implemented: `box, sphere, cylinder, cone`

Check the following messages definitions:
| Msg                                      | Src                                                                                             |
| ---------------------------------------- | ----------------------------------------------------------------------------------------------- |
| `moveit_msgs/msg/CollisionObject`        | [link](https://github.com/ros-planning/moveit_msgs/blob/humble/msg/CollisionObject.msg)         |
| `object_recognition_msgs/msg/ObjectType` | [link](https://github.com/wg-perception/object_recognition_msgs/blob/ros2/msg/ObjectType.msg)   |
| `shape_msgs/msg/SolidPrimitive.msg`      | [link](https://github.com/ros2/common_interfaces/blob/humble/shape_msgs/msg/SolidPrimitive.msg) |


## Libraries used
- [argparse](https://github.com/p-ranav/argparse) (MIT License)
