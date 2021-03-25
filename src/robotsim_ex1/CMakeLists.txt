cmake_minimum_required(VERSION 3.0.2)
project(robotsim_ex1)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS message_generation roscpp rospy std_msgs geometry_msgs)

## Service files
add_service_files(
  FILES
  SetTarget.srv
  DistanceToTarget.srv
)

## Generate added messages and services
generate_messages(
    DEPENDENCIES
    std_msgs
    geometry_msgs
)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(random_publisher src/random_publisher.cpp)
target_link_libraries(random_publisher ${catkin_LIBRARIES})
# add_dependencies(random_publisher robotsim_ex1_generate_messages_cpp)

add_executable(quadrant_subscriber src/quadrant_subscriber.cpp)
target_link_libraries(quadrant_subscriber ${catkin_LIBRARIES})
# add_dependencies(quadrant_subscriber robotsim_ex1_generate_messages_cpp)

add_executable(set_target src/set_target.cpp)
target_link_libraries(set_target ${catkin_LIBRARIES})
add_dependencies(set_target robotsim_ex1_gencpp)

