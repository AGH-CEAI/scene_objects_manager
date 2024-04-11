#include "scene_objects_spawner/scene_objects_spawner.hpp"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>

#include "scene_objects_spawner/object_spawner.hpp"
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

  auto const node = std::make_shared<SceneObjectsSpawner>();
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  auto spinner = std::thread([&executor]() { executor.spin(); });

  spawner = ObjectsSpawner("world");
  for (const auto& obj : scene_objs) {
    spanwer.spawn_object(obj);
    RCLCPP_INFO(node->get_logger(),
                "Spawned new scene object \"" << obj.pretty_name << "\" (unique_id: " << obj.unique_id << " ).");
  }

  // rclcpp::spin();
  rclcpp::shutdown();
  spinner.join();
  return 0;
}
