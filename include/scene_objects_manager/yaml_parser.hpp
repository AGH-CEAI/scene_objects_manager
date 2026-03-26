#ifndef SCENE_OBJECTS_MANAGER__YAML_PARSER_HPP_
#define SCENE_OBJECTS_MANAGER__YAML_PARSER_HPP_
#include <filesystem>
#include <vector>

#include "scene_objects_manager/scene_object.hpp"

namespace sobjmanager {

std::vector<SceneObject> load_scene_objects_from_yaml(std::filesystem::path path);

}  // namespace sobjmanager
#endif  // SCENE_OBJECTS_MANAGER__YAML_PARSER_HPP_
