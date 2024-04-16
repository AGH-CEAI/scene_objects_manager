#include "scene_objects_manager/scene_objects_manager.hpp"

#include "scene_objects_manager/argv_parser.hpp"
#include "scene_objects_manager/spawner.hpp"
#include "scene_objects_manager/yaml_parser.hpp"

using namespace sobjspawner;

int main(int argc, char* argv[]) {
  const LaunchArguments launch_args = [&] {
    std::vector<std::string> args = rclcpp::init_and_remove_ros_arguments(argc, argv);
    return parse_args("SceneObjectsSpawner", "0.0.0", args);
  }();
  auto const node = std::make_shared<SceneObjectsSpawner>();

  auto scene_objs = load_scene_objects_from_yaml(launch_args.cfg_path);
  RCLCPP_INFO(
      node->get_logger(),
      "Loaded %d object(s) from the configuration file.",
      static_cast<int>(scene_objs.size()));

  auto spawner = Spawner(launch_args.planning_frame_id);
  for (const auto& obj : scene_objs) {
    spawner.spawn_object(obj);
    RCLCPP_INFO(
        node->get_logger(),
        "Spawned new scene object \"%s\" (unique_id: %s).",
        obj.pretty_name.c_str(),
        obj.unique_id.c_str());
  }
  RCLCPP_INFO(node->get_logger(), "Spawned all objects.");

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
