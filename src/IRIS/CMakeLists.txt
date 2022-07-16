cmake_minimum_required(VERSION 3.0.2)
project(IRIS)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  rosbridge_server
  roscpp
  rospy
  std_msgs
  image_transport
  message_generation
)
find_package(OpenCV REQUIRED)

## MENAMBHAKAN MSG dan SRV FILE
add_message_files(FILES BSTX.msg BSRX.msg)
  
## Generate added messages and services
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
# INCLUDE_DIRS include
# LIBRARIES IRIS
# CATKIN_DEPENDS cv_bridge rosbridge_server roscpp rospy std_msgs image_transport
# DEPENDS system_lib 
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

# install(DIRECTORY include/${IRIS}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   PATTERN ".h" EXCLUDE
# )

# add_executable(node1 src/node1.cpp)
# target_link_libraries(node1 ${catkin_LIBRARIES} ${OpenCV_INCLUDE_DIRS} )
# add_dependencies(node1 ${${IRIS}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(node2 src/node2.cpp)
target_link_libraries(node2 ${catkin_LIBRARIES} )
add_dependencies(node2 ${${IRIS}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# add_executable(node3 src/node3.cpp)
# target_link_libraries(node3 ${catkin_LIBRARIES} )
# add_dependencies(node3 ${${IRIS}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})