#include "ros/ros.h"
#include "assignment1/ErrorToTwist.h"
#include "geometry_msgs/Twist.h"
// Derivative variables
uint16_t prevDistance = 0;
// int16_t prevAngle = 0;


// Integral variables
uint16_t totalDistance = 0;
// int16_t totalAngle = 0;

bool isSeen(uint16_t distance) 
{
  if (distance < 65535) return true;
  else return false;
}

geometry_msgs::Vector3 calcLinear(uint16_t distance) 
{
  geometry_msgs::Vector3 linear;
  double speed;

  totalDistance += distance;
  speed = 0.01 * (0.1 * distance + (distance - prevDistance) + 0.001 * totalDistance);
  prevDistance = distance;
  if (isSeen(distance)) 
  {
    linear.x = speed;
  }
  else {
    // Reset integral when out of site
    totalDistance = 0;
    linear.x = 0.0;
  }
  linear.y = 0.0;
  linear.z = 0.0;
  return linear;
}

geometry_msgs::Vector3 calcAngle(int16_t angle) 
{
  geometry_msgs::Vector3 angular;
  double rotation;

  // Inverse rotation
  rotation = -0.05 * angle;

  angular.x = 0.0;
  angular.y = 0.0;
  angular.z = rotation;
  return angular;
}

bool calcTwist (
  assignment1::ErrorToTwist::Request &req,
  assignment1::ErrorToTwist::Response &res
)
{
  geometry_msgs::Twist twist;
  twist.linear = calcLinear(req.error.distance);
  twist.angular = calcAngle(req.error.angle);
  
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

