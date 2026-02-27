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
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rosidl_default_generators REQUIRED)

include(FetchContent)
FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

set(
  SOURCE_FILES
  src/args_parser.cpp
  src/scene_objects_manager.cpp
  src/spawner.cpp
  src/yaml_parser.cpp
  src/object_pose_detector.cpp
)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/DetectBlocksPoses.srv"
  DEPENDENCIES geometry_msgs
)

add_executable(${PROJECT_NAME}_node ${SOURCE_FILES})

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} "rosidl_typesupport_cpp"
)

target_link_libraries(${PROJECT_NAME}_node
  ${cpp_typesupport_target}
)

ament_target_dependencies(
  ${PROJECT_NAME}_node
  moveit_ros_planning_interface
  rclcpp
  std_msgs
  rclcpp
  sensor_msgs
  geometry_msgs
  cv_bridge
)

target_include_directories(
  ${PROJECT_NAME}_node PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
  ${argparse_SOURCE_DIR}/argparse/include
)

target_link_libraries(
  ${PROJECT_NAME}_node
  argparse
  yaml-cpp
  ${OpenCV_LIBRARIES}
)

install(
  TARGETS ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_dependencies(rosidl_default_runtime)
ament_package()
