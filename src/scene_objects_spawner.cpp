#include "scene_objects_spawner/scene_objects_spawner.hpp"

#include "yaml-cpp/yaml.h"

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
  // TODO(macale) pass path to YAML config as argv, fallback to nothing
  YAML::Node config = YAML::LoadFile("/home/macale/ceai/ros_ws/src/scene_objects_spawner/test/config.yml");

  // TODO(macale) write type parser according to yaml-cpp & MoveIt2 docs
  for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
    auto key = it->first.as<std::string>();
    std::cout << "SHAPE:" << config[key]["type"] << " | Name: " << config[key]["pretty_name"] << "\n";
  }

  std::cout << std::endl;

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SceneObjectsSpawner>());
  rclcpp::shutdown();
  return 0;
}
