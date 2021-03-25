cmake_minimum_required(VERSION 3.0.2)
project(robotsim_ex1)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs geometry_msgs)

## Service files
add_service_files(
  FILES
  DistanceToTarget.srv
  SetTarget.srv
)


## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs geometry_msgs)

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
