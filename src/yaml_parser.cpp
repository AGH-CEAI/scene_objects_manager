#include "scene_objects_spawner/yaml_parser.hpp"

#include <yaml-cpp/yaml.h>

std::vector<SceneObject> load_scene_objects_from_yaml(std::filesystem::path path) {
  YAML::Node cfg = YAML::LoadFile(path.string());

  std::vector<SceneObject> scene_objects;
  scene_objects.reserve(cfg.size());

  for (YAML::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
    auto unique_id = it->first.as<std::string>();

    SceneObject obj = cfg[unique_id].as<SceneObject>();
    obj.unique_id = unique_id;

    scene_objects.push_back(obj);
  }

  return scene_objects;
}
