#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

// Random number generation
#include <stdlib.h> 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "random_publisher");
  ros::NodeHandle n;
  ros::Publisher random_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
  // Low loop rate for smoother input
  ros::Rate loop_rate(1);

  while (ros::ok())
  {
    geometry_msgs::Twist twist;

    twist.linear.x = 0.5;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;

    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    // Random angle to steer robot
    twist.angular.z = (rand() % 100 - 50) / 25;

    random_pub.publish(twist);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}