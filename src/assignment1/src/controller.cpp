#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "assignment1/Sonars.h"
#include "assignment1/SonarsToError.h"
#include "assignment1/ErrorToTwist.h"

class Controller {
  public:
    ros::NodeHandle n;
    assignment1::Sonars lastReading;
    // Callback for sonars subscriber assigns message to Sonars
    void sonarsCallback(const assignment1::Sonars);
    // Calculate twist for publisher
    geometry_msgs::Twist calcTwist();
};

void Controller::sonarsCallback(const assignment1::Sonars sonars)
{
  lastReading = sonars;
}

geometry_msgs::Twist Controller::calcTwist() {
  geometry_msgs::Twist return_twist;
  // Calculate Error
  ros::ServiceClient errorClient = n.serviceClient<assignment1::SonarsToError>("sonars_to_error");
  assignment1::SonarsToError errorSrv;
  errorSrv.request.sonars = lastReading;
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
    ROS_ERROR("Failed to call service SonarsToError");
    return return_twist;
  }
}

int main(int argc, char **argv)
{
  // Init
  ros::init(argc, argv, "controller");
  Controller controller;
  // Subscribe to sonars
  ros::Subscriber sub = controller.n.subscribe("sonars", 1000, &Controller::sonarsCallback, &controller);
  // Calculate twist and publish
  ros::Publisher twist_pub = controller.n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  while(ros::ok()) {
    // Calculate Twist
    geometry_msgs::Twist twist_command = controller.calcTwist();
    ROS_INFO("Speed: %f %f %f", twist_command.linear.x, twist_command.linear.y, twist_command.linear.z);
    ROS_INFO("Angle: %f %f %f", twist_command.angular.x, twist_command.angular.y, twist_command.angular.z);
    // Publish message
    twist_pub.publish(twist_command);
    ros::spinOnce();
  }
  return 0;
}