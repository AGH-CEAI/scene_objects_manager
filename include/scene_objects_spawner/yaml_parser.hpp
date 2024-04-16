#ifndef YAML_PARSER_HPP_INCLUDED
#define YAML_PARSER_HPP_INCLUDED
#include <filesystem>
#include <vector>

#include "scene_objects_spawner/scene_object.hpp"

std::vector<SceneObject> load_scene_objects_from_yaml(std::filesystem::path path);
#endif  // YAML_PARSER_HPP_INCLUDED
