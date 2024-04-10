#include "scene_objects_spawner/scene_objects_spawner.hpp"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>

#include "scene_objects_spawner/yaml_parser.hpp"

SceneObjectsSpawner::SceneObjectsSpawner() : Node("scene_obecjts_spawner"), count_(0) {
  publisher_ = this->create_publisher<MyAdaptedType>("topic", 10);
  timer_ = this->create_wall_timer(500ms, std::bind(&SceneObjectsSpawner::timer_callback, this));
}

void SceneObjectsSpawner::timer_callback() {
  std::string message = "Hello, world! " + std::to_string(count_++);
  RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.c_str());
  publisher_->publish(message);
}

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  // TODO(macale) pass path to YAML config as argv, fallback to nothing
  std::filesystem::path yaml_cfg("/home/macale/ceai/ros_ws/src/scene_objects_spawner/test/config.yml");
  auto scene_objs = load_scene_objects_from_yaml(yaml_cfg);

  // TODO(macale) write type parser according to yaml-cpp & MoveIt2 docs
  for (const auto& obj : scene_objs) {
    std::cout << "LOADED:" << obj.unique_id_id << " | Name: " << obj.pretty_name << "\n";
  }

  std::cout << std::endl;

  auto const node = std::make_shared<SceneObjectsSpawner>();
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  auto spinner = std::thread([&executor]() { executor.spin(); });

  // Create the MoveIt MoveGroup Interface
  using moveit::planning_interface::MoveGroupInterface;
  // TODO(macale) paramtetrize move_group name from argv
  auto move_group_interface = MoveGroupInterface(node, "ur_arm");

  // Create collision object for the robot to avoid
  auto const collision_object = [frame_id = move_group_interface.getPlanningFrame()] {
    moveit_msgs::msg::CollisionObject collision_object;
    collision_object.header.frame_id = frame_id;
    collision_object.id = "box1";
    shape_msgs::msg::SolidPrimitive primitive;

    // Define the size of the box in meters
    primitive.type = primitive.BOX;
    primitive.dimensions.resize(3);
    primitive.dimensions[primitive.BOX_X] = 0.5;
    primitive.dimensions[primitive.BOX_Y] = 0.1;
    primitive.dimensions[primitive.BOX_Z] = 0.5;

    // Define the pose of the box (relative to the frame_id)
    geometry_msgs::msg::Pose box_pose;
    box_pose.orientation.w =
        1.0;  // We can leave out the x, y, and z components of the quaternion since they are initialized to 0
    box_pose.position.x = 0.4;
    box_pose.position.y = 0.4;
    box_pose.position.z = 0.25;

    collision_object.primitives.push_back(primitive);
    collision_object.primitive_poses.push_back(box_pose);
    collision_object.operation = collision_object.ADD;

    return collision_object;
  }();

  // rclcpp::spin();
  rclcpp::shutdown();
  spinner.join();
  return 0;
}
