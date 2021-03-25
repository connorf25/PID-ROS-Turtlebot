#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Point.h"

geometry_msgs::Point current_position;

void position_callback(const nav_msgs::Odometry odom) {
  current_position = odom.pose.pose.position;
}

int main(int argc, char **argv)
{
  // Init
  ros::NodeHandle n;
  ros::init(argc, argv, "controller");
  // Subscribe to sonars
  ros::Subscriber sub = n.subscribe("odom", 1000, position_callback);
}