#pragma once
#include <filesystem>
#include <string>
#include <vector>

struct LaunchArguments {
  std::filesystem::path cfg_path;
  std::string planning_frame_id;
};

bool validate_launch_arguments(const LaunchArguments& args);

const LaunchArguments parse_args(
    const std::string& program_name,
    const std::string& program_version,
    const std::vector<std::string>& input_args);
