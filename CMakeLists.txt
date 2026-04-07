cmake_minimum_required(VERSION 3.8)
project(scene_objects_manager)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(ROS_DEPENDS
    cv_bridge
    geometry_msgs
    moveit_msgs
    moveit_ros_planning_interface
    rosidl_default_generators
    std_msgs
    sensor_msgs
    tf2
    tf2_ros
    tf2_geometry_msgs)
set(PROJECT_DEPENDS OpenCV yaml-cpp ${ROS_DEPENDS})

find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(rclcpp REQUIRED)

foreach(dependency IN ITEMS ${PROJECT_DEPENDS})
  find_package(${dependency} REQUIRED)
endforeach()

include(FetchContent)
FetchContent_Declare(argparse
                     GIT_REPOSITORY https://github.com/p-ranav/argparse.git)
FetchContent_MakeAvailable(argparse)

set(SOURCE_FILES
    src/args_parser.cpp src/scene_objects_manager.cpp src/spawner.cpp
    src/yaml_parser.cpp src/object_pose_detector.cpp)

rosidl_generate_interfaces(${PROJECT_NAME} "srv/DetectBlocksPoses.srv"
                           DEPENDENCIES geometry_msgs)

add_executable(${PROJECT_NAME}_node ${SOURCE_FILES})

rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME}
                              "rosidl_typesupport_cpp")

target_link_libraries(${PROJECT_NAME}_node ${cpp_typesupport_target})

ament_target_dependencies(${PROJECT_NAME}_node ament_index_cpp rclcpp
                          ${ROS_DEPENDS})

target_include_directories(
  ${PROJECT_NAME}_node
  PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
         "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
         ${argparse_SOURCE_DIR}/argparse/include)

target_link_libraries(${PROJECT_NAME}_node argparse yaml-cpp
                      ${OpenCV_LIBRARIES})

install(TARGETS ${PROJECT_NAME}_node DESTINATION lib/${PROJECT_NAME})

ament_export_dependencies(rosidl_default_runtime)
ament_package()
