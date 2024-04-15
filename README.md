# Scene Objects Spawner

A Node for spawning objects in MoveIt2 environment with ROS 2 API.

(If more features will be introducede, the package name will be changed).

---

TODO - implementation of the following is needed.


Base the implementation on this type from [MoveIt2 Docs](https://moveit.picknik.ai/main/doc/tutorials/planning_around_objects/planning_around_objects.html#create-a-collision-object).
Objects types: `box, sphere, cylinder, cone, mesh_from_file, mesh_from_url`

Check the following messages definitions:
| Msg                                      | Src                                                                                             |
| ---------------------------------------- | ----------------------------------------------------------------------------------------------- |
| `moveit_msgs/msg/CollisionObject`        | [link](https://github.com/ros-planning/moveit_msgs/blob/humble/msg/CollisionObject.msg)         |
| `object_recognition_msgs/msg/ObjectType` | [link](https://github.com/wg-perception/object_recognition_msgs/blob/ros2/msg/ObjectType.msg)   |
| `shape_msgs/msg/SolidPrimitive.msg`      | [link](https://github.com/ros2/common_interfaces/blob/humble/shape_msgs/msg/SolidPrimitive.msg) |

Consider future connection with `vision_msgs` ([link](https://github.com/ros-perception/vision_msgs)).

Especially, consider if the following yaml files should be oneday somehow connected with database.

## Libraries used
- [argparse](https://github.com/p-ranav/argparse) (MIT License)
