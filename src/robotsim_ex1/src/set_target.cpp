#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "robotsim_ex1/SetTarget.h"

bool random_target(
    robotsim_ex1::SetTarget::Request &req,
    robotsim_ex1::SetTarget::Response &res
) 
{
    geometry_msgs::Vector3 target;
    target.x = (rand() % 100 - 50);
    target.y = (rand() % 100 - 50);
    target.z = (rand() % 100 - 50);
    res.target_location = target;
    return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "set_target");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("set_target", distance_to_target);
  ros::spin();

  return 0;
}