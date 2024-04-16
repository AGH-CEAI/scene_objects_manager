cmake_minimum_required(VERSION 3.8)
project(scene_objects_manager)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(std_msgs REQUIRED)
find_package(yaml-cpp REQUIRED)
include(FetchContent)
FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

set(
  SOURCE_FILES
  src/argv_parser.cpp
  src/scene_objects_manager.cpp
  src/spawner.cpp
  src/yaml_parser.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
ament_target_dependencies(
  ${PROJECT_NAME}
  moveit_ros_planning_interface
  rclcpp
  std_msgs
)

target_include_directories(
  ${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
  ${argparse_SOURCE_DIR}/argparse/include
)

target_link_libraries(
  ${PROJECT_NAME}
  argparse
  yaml-cpp
)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
