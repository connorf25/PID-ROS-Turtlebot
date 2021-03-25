#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "robotsim_ex1/DistanceToTarget.h"
#include <math.h> 

bool distance_to_target(
    robotsim_ex1::DistanceToTarget::Request &req,
    robotsim_ex1::DistanceToTarget::Response &res
) 
{
    geometry_msgs::Vector3 target = req.target_position;
    geometry_msgs::Vector3 position;
    double distance;
    position.x = req.current_position.x;
    position.y = req.current_position.y;
    position.z = req.current_position.z;
    distance = sqrt(pow((target.x - position.x), 2) + pow((target.y - position.y), 2) + pow((target.z - position.z), 2));

    res.relative_distance = distance;
    res.robot_location = position;
    res.target_location = target;
    return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "distance_to_target");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("distance_to_target", distance_to_target);
  ros::spin();

  return 0;
}