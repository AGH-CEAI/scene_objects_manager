#include "scene_objects_manager/args_parser.hpp"

#include <argparse/argparse.hpp>

namespace sobjmanager {

bool validate_launch_arguments(const LaunchArguments& args) {
  if (args.cfg_path.string().empty()) {
    std::cerr << "No given path to a configuration file." << std::endl;
    return false;
  }

  return true;
}

const LaunchArguments parse_args(
    const std::string& program_name,
    const std::string& program_version,
    const std::vector<std::string>& input_args) {
  argparse::ArgumentParser program(program_name, program_version, argparse::default_arguments::help);
  LaunchArguments args;

  program.add_argument("-f", "--frame")
      .store_into(args.planning_frame_id)
      .default_value("world")
      .help("planning frame id.")
      .nargs(1);

  std::string cfg_path;
  program.add_argument("-c", "--cfg").store_into(cfg_path).help("the path to the YAML configuration file.");

  try {
    program.parse_args(input_args);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl << program;
    std::exit(1);
  }

  args.cfg_path = std::filesystem::path(cfg_path);

  if (!validate_launch_arguments(args)) {
    std::cout << "Tip: check usage with the --help flag. \n";
    std::exit(1);
  }

  return args;
}

}  // namespace sobjmanager
