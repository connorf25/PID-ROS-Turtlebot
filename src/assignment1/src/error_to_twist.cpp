#include "ros/ros.h"
#include "assignment1/ErrorToTwist.h"
#include "geometry_msgs/Twist.h"

// Change of Time
// Delta T is 1/loop_rate
double deltaT = 1.0/100.0; 

// Derivative variables
uint16_t prevDistance = 0;
// Integral variables
double integralDistance = 0.0;

bool isSeen(uint16_t distance) 
{
  if (distance < 65535) return true;
  else return false;
}

geometry_msgs::Vector3 calcLinear(uint16_t distance) 
{
  geometry_msgs::Vector3 linear;
  double speed, p, i, d;

  integralDistance += (distance * deltaT);
  p = 0.5 * distance;
  i = 0.01 * integralDistance;
  d = 1.0 * (distance - prevDistance) / deltaT;
  speed = 0.01 * (p + i + d);
  ROS_INFO("%lf", speed);
  prevDistance = distance;
  if (isSeen(distance)) 
  {
    linear.x = std::min(speed, 0.22);
  }
  else {
    // Reset integral and stop moving when bowl out of sight
    integralDistance = 0;
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

  // Inverse rotation and calculate as proportional
  rotation = std::min(-0.05 * angle, 2.84);

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

