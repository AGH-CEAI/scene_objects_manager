#ifndef ARGS_PARSER_HPP_INCLUDED
#define ARGS_PARSER_HPP_INCLUDED

#include <filesystem>
#include <string>
#include <vector>

namespace sobjmanager {

struct LaunchArguments {
  std::filesystem::path cfg_path;
  std::string planning_frame_id;
};

bool validate_launch_arguments(const LaunchArguments& args);

const LaunchArguments parse_args(
    const std::string& program_name,
    const std::string& program_version,
    const std::vector<std::string>& input_args);

}  // namespace sobjmanager
#endif  // ARGS_PARSER_HPP_INCLUDED
