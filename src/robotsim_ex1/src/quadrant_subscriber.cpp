#include "ros/ros.h"
#include "math.h"
#include "turtlesim/Pose.h"

void quadrant_callback(const turtlesim::Pose pose)
{
  // Take position from pose publisher
  double x, y, angle;
  // Subtract 5 to make starting position origin
  x = pose.x - 5.44444;
  y = pose.y - 5.44444;
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
  ros::Subscriber sub = n.subscribe("/turtle1/pose", 1000, quadrant_callback);
  ros::spin();

  return 0;
}