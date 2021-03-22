#include "ros/ros.h"
#include "assignment1/ErrorToTwist.h"
#include "geometry_msgs/Twist.h"

bool calcTwist (
  assignment1::ErrorToTwist::Request &req,
  assignment1::ErrorToTwist::Response &res
)
{
  uint16_t distance = req.error.distance;
  geometry_msgs::Twist twist;
  double speed = 0.0;
  
  speed = 0.1 * distance;

  twist.linear.x = speed;
  twist.linear.y = 0.0;
  twist.linear.z = 0.0;

  twist.angular.x = 0.0;
  twist.angular.y = 0.0;
  twist.angular.z = 0.0;

  res.twist = twist;
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "error_to_twist");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("error_to_twist", calcTwist);
  ros::spin();

  return 0;
}

