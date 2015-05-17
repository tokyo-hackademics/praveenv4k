cmake_minimum_required(VERSION 2.8.3)
project(hri)

find_package(catkin REQUIRED COMPONENTS
  diagnostic_msgs
  rospy
  roscpp
  rospeex_msgs
  rospeex_if
  roslib
  message_generation
)

find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs rospeex_msgs rospeex_if roslib
  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp executable
add_executable(hri src/hri.cpp)

add_dependencies(hri ${catkin_EXPORTED_TARGETS})

target_link_libraries(hri ${catkin_LIBRARIES})

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
  PATTERN ".svn" EXCLUDE
)

install(TARGETS hri 
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
