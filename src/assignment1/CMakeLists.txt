cmake_minimum_required(VERSION 3.0.2)
project(assignment1)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  geometry_msgs
)


# Generate messages in the 'msg' folder
add_message_files(
  FILES
  Error.msg
  Sonars.msg
)

# Generate services in the 'srv' folder
add_service_files(
  FILES
  GetError.srv
  ErrorToTwist.srv
  KalmanFilter.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )


# Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES assignment1
#  CATKIN_DEPENDS message_generation roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(get_error src/get_error.cpp)
target_link_libraries(get_error ${catkin_LIBRARIES})
add_dependencies(get_error assignment1_gencpp)

add_executable(error_to_twist src/error_to_twist.cpp)
target_link_libraries(error_to_twist ${catkin_LIBRARIES})
add_dependencies(error_to_twist assignment1_gencpp)

add_executable(controller src/controller.cpp)
target_link_libraries(controller ${catkin_LIBRARIES})
add_dependencies(controller assignment1_gencpp)

add_executable(noisy_controller src/controller.cpp)
target_compile_definitions(noisy_controller PUBLIC -DNOISY_SONAR)
target_link_libraries(noisy_controller ${catkin_LIBRARIES})
add_dependencies(noisy_controller assignment1_gencpp)

add_executable(calc_variance src/calc_variance.cpp)
target_link_libraries(calc_variance ${catkin_LIBRARIES})
add_dependencies(calc_variance assignment1_gencpp)

add_executable(kalman_filter src/kalman_filter.cpp)
target_link_libraries(kalman_filter ${catkin_LIBRARIES})
add_dependencies(kalman_filter assignment1_gencpp)