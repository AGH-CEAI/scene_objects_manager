#include <filesystem>
#include <vector>

#include "scene_objects_spawner/scene_object.hpp"

using SolidPrimitive = shape_msgs::msg::SolidPrimitive;

std::vector<SceneObject> load_scene_objects_from_yaml(std::filesystem::path path);

namespace YAML {
static const inline std::map<std::string, uint8_t> OBJECT_TYPE_MAP = {
  { "box", SolidPrimitive::BOX },   { "sphere", SolidPrimitive::SPHERE }, { "cylinder", SolidPrimitive::CYLINDER },
  { "cone", SolidPrimitive::CONE }, { "prism", SolidPrimitive::PRISM },
};

template <>
struct convert<SceneObject> {
  static Node encode(const SceneObject&) {
    throw std::runtime_error("Conversion of SceneObject to YAML is not implemented.");
    return Node();
  }

  static bool decode(const Node& node, SceneObject& rhs) {
    if (!node.IsMap() || node.size() != 6) {
      return false;
    }

    // Can't obtain the unique_id from the passed node.
    rhs.pretty_name = node["pretty_name"].as<std::string>();
    rhs.frame_id = node["attach_to_frame"].as<std::string>();

    rhs.type = [&] {
      auto type_str = node["type"].as<std::string>();
      std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);
      return OBJECT_TYPE_MAP.at(type_str);
    }();

    rhs.size = node["size"].as<geometry_msgs::msg::Point>();
    rhs.scale = node["scale"].as<double>();
    rhs.pose = node["pose"].as<geometry_msgs::msg::Pose>();

    return true;
  }
};

template <>
struct convert<geometry_msgs::msg::Pose> {
  static Node encode(const geometry_msgs::msg::Pose& rhs) {
    Node node;
    node["position"] = rhs.position;
    node["orientation"] = rhs.orientation;
    return node;
  }

  static bool decode(const Node& node, geometry_msgs::msg::Pose& rhs) {
    if (!node.IsMap() || node.size() != 2) {
      return false;
    }

    rhs.position = node["position"].as<geometry_msgs::msg::Point>();
    rhs.orientation = node["orientation"].as<geometry_msgs::msg::Quaternion>();
    return true;
  }
};

template <>
struct convert<geometry_msgs::msg::Point> {
  static Node encode(const geometry_msgs::msg::Point& rhs) {
    Node node;
    node["x"] = rhs.x;
    node["y"] = rhs.y;
    node["z"] = rhs.z;
    return node;
  }

  static bool decode(const Node& node, geometry_msgs::msg::Point& rhs) {
    if (!node.IsMap() || node.size() != 3) {
      return false;
    }

    rhs.x = node["x"].as<double>();
    rhs.y = node["y"].as<double>();
    rhs.z = node["z"].as<double>();
    return true;
  }
};

template <>
struct convert<geometry_msgs::msg::Quaternion> {
  static Node encode(const geometry_msgs::msg::Quaternion& rhs) {
    Node node;
    node["x"] = rhs.x;
    node["y"] = rhs.y;
    node["z"] = rhs.z;
    node["w"] = rhs.w;
    return node;
  }

  static bool decode(const Node& node, geometry_msgs::msg::Quaternion& rhs) {
    if (!node.IsMap() || node.size() != 4) {
      return false;
    }

    rhs.x = node["x"].as<double>();
    rhs.y = node["y"].as<double>();
    rhs.z = node["z"].as<double>();
    rhs.w = node["w"].as<double>();
    return true;
  }
};

}  // namespace YAML
