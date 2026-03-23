# Scene Objects Manager

A Node for object management in MoveIt2 Planning Scene environment with ROS 2 API.

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


## Block detection service (based on ArUco markers)
This service detects blocks marked with ArUco markers using scene camera and returns their poses.

### Service description
The `/detect_blocks_poses` service provides the poses of all detected blocks in the scene.
- Service name: `detect_blocks_poses`
- Service type: `scene_objects_manager/srv/DetectBlocksPoses`
- Response: `geometry_msgs/msg/PoseArray`

The response contains a list of poses (`PoseArray`), where each pose corresponds to a detected block in the camera frame.

### Example usage:
```bash
ros2 service call /detect_blocks_poses scene_objects_manager/srv/DetectBlocksPoses "{detect: true}"
```

## Libraries used
- [argparse](https://github.com/p-ranav/argparse) (MIT License)

## Tests
- `detect_all_markers.py` - is publishing static transform to the tf of detected blocks.
- `go_above_markers.py` - is moving the robot above the detected markers.
