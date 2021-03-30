#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Vector3.h"
#include "robotsim_ex1/SetTarget.h"
#include "robotsim_ex1/DistanceToTarget.h"

geometry_msgs::Vector3 current_position;

void position_callback(const turtlesim::Pose pose) {
    current_position.x = pose.x;
    current_position.y = pose.y;
}

int main(int argc, char **argv)
{
    // Init
    ros::init(argc, argv, "client");
    ros::NodeHandle n;
    // Subscribe to pose
    ros::Subscriber sub = n.subscribe("/turtle1/pose", 1000, position_callback);
    // Set target
    geometry_msgs::Vector3 target;
    ros::ServiceClient set_target = n.serviceClient<robotsim_ex1::SetTarget>("set_target");
    robotsim_ex1::SetTarget set_target_srv;
    if (set_target.call(set_target_srv)) {
        target = set_target_srv.response.target_location;
    } else {
        ROS_ERROR("FAILED TO SET TARGET");
    }
    // Setup Distance Service
    ros::ServiceClient distance_to_target = n.serviceClient<robotsim_ex1::DistanceToTarget>("distance_to_target");
    robotsim_ex1::DistanceToTarget distance_to_target_srv;
    ros::Rate loop_rate(1);
    while(ros::ok()) {
        // Request variables
        distance_to_target_srv.request.current_position = current_position;
        distance_to_target_srv.request.target_position = target;

        if (distance_to_target.call(distance_to_target_srv)) {
            ROS_INFO("Distance %f", distance_to_target_srv.response.relative_distance);
            ROS_INFO(
                "Robot: %f %f",
                distance_to_target_srv.response.robot_location.x,
                distance_to_target_srv.response.robot_location.y
            );
            ROS_INFO(
                "Target: %f %f",
                distance_to_target_srv.response.target_location.x,
                distance_to_target_srv.response.target_location.y
            );
        } else {
            ROS_ERROR("FAILED TO SET TARGET");
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
}