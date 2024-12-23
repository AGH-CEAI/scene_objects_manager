#ifndef YAML_PARSER_HPP_INCLUDED
#define YAML_PARSER_HPP_INCLUDED
#include <filesystem>
#include <vector>

#include "scene_objects_manager/scene_object.hpp"

namespace sobjmanager {

std::vector<SceneObject> load_scene_objects_from_yaml(std::filesystem::path path);

}  // namespace sobjmanager
#endif  // YAML_PARSER_HPP_INCLUDED
