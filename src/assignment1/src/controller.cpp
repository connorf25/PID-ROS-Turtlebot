#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "assignment1/Sonars.h"
#include "assignment1/GetError.h"
#include "assignment1/ErrorToTwist.h"

class Controller {
  public:
    ros::NodeHandle n;
    // Calculate twist for publisher
    geometry_msgs::Twist calcTwist();
};

geometry_msgs::Twist Controller::calcTwist() {
  geometry_msgs::Twist return_twist;
  // Calculate Error
  ros::ServiceClient errorClient = n.serviceClient<assignment1::GetError>("get_error");
  assignment1::GetError errorSrv;
  if (errorClient.call(errorSrv))
  {
    // Calculate Twist
    ros::ServiceClient twistClient = n.serviceClient<assignment1::ErrorToTwist>("error_to_twist");
    assignment1::ErrorToTwist twistSrv;
    twistSrv.request.error = errorSrv.response.error;
    if (twistClient.call(twistSrv))
    {
      return_twist = twistSrv.response.twist;
      return return_twist;
    } 
    else 
    {
      ROS_ERROR("Failed to call service ErrorToTwist");
      return return_twist;
    }
  }
  else
  {
    ROS_ERROR("Failed to call service GetError");
    return return_twist;
  }
}

int main(int argc, char **argv)
{
  // Init
  ros::init(argc, argv, "controller");
  Controller controller;
  // Calculate twist and publish
  ros::Publisher twist_pub = controller.n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Rate loop_rate(100);
  while(ros::ok()) {
    // Calculate Twist
    geometry_msgs::Twist twist_command = controller.calcTwist();
    // Publish message
    twist_pub.publish(twist_command);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}