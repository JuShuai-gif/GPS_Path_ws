cmake_minimum_required(VERSION 3.0.2)
project(gps_path_puh)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package (GeographicLib REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  std_msgs
)


catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES gps_path_puh
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GeographicLib_INCLUDE_DIRS}
)
include_directories(/usr/include/eigen3/)

add_executable(gps_path_publisher_node src/gps_path_publisher_node.cpp)

target_link_libraries(gps_path_publisher_node
  ${catkin_LIBRARIES}
  ${GeographicLib_LIBRARIES}
)