#pragma once
#include <filesystem>
#include <vector>

#include "scene_objects_manager/scene_object.hpp"

std::vector<SceneObject> load_scene_objects_from_yaml(std::filesystem::path path);

namespace YAML {
template <>
struct convert<SceneObject> {
  static Node encode(const SceneObject& rhs) {
    Node node;
    node["pretty_name"] = rhs.pretty_name;
    node["frame_id"] = rhs.frame_id;
    node["type"] = PRIMITIVE_UINT_MAP.at(rhs.type);
    node["size"] = rhs.size;
    node["scale"] = rhs.scale;
    node["pose"] = rhs.pose;
    return node;
  }

  static bool decode(const Node& node, SceneObject& rhs) {
    if (!node.IsMap() || node.size() != 6) {
      return false;
    }

    // Can't obtain the unique_id from the passed node.
    rhs.pretty_name = node["pretty_name"].as<std::string>();
    rhs.frame_id = node["frame_id"].as<std::string>();

    rhs.type = [&] {
      auto type_str = node["type"].as<std::string>();
      std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);
      return PRIMITIVE_STR_MAP.at(type_str);
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
