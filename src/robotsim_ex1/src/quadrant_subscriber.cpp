#include "ros/ros.h"
#include "math.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Point.h"

void quadrant_callback(const nav_msgs::Odometry odom)
{
  // Take point from odom publisher
  geometry_msgs::Point point = odom.pose.pose.position;
  double x, y, angle;
  x = point.x;
  y = point.y;
  // Calculate angle
  angle = atan2(y, x) * 180 / M_PI;
  // Correction if angle is < 0 and goes negative
  if (angle < 0) {
    angle = 360 + angle;
  }
  if (angle >= 0 && angle < 90) 
  {
    ROS_INFO("position: [%f, %f]  angle: %f (QUADRANT 1)", x, y, angle);
  }
  else if (angle >= 90 && angle < 180) 
  {
    ROS_INFO("position: [%f, %f]  angle: %f (QUADRANT 2)", x, y, angle);
  }
  else if (angle >= 180 && angle < 270) 
  {
    ROS_INFO("position: [%f, %f]  angle: %f (QUADRANT 3)", x, y, angle);
  }
  else if (angle >= 270 && angle < 360) 
  {
    ROS_INFO("position: [%f, %f]  angle: %f (QUADRANT 4)", x, y, angle);
  }
  else
  {
    ROS_INFO("Invalid angle: %f", angle);
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "quadrant_subscriber");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("odom", 1000, quadrant_callback);
  ros::spin();

  return 0;
}