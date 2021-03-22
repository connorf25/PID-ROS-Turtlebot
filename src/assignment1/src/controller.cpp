#include "ros/ros.h"
#include "assignment1/Sonars.h"
#include "assignment1/SonarsToError.h"
#include "assignment1/ErrorToTwist.h"

class Controller {
  public:
    ros::NodeHandle n;
    void sonarsCallback(const assignment1::Sonars);
};

void Controller::sonarsCallback(const assignment1::Sonars sonars)
{
  // Calculate Error
  ros::ServiceClient errorClient = n.serviceClient<assignment1::SonarsToError>("sonars_to_error");
  assignment1::SonarsToError errorSrv;
  errorSrv.request.sonars = sonars;
  if (errorClient.call(errorSrv))
  {
    ROS_INFO("Distance: %li, Angle : %li", (long int)errorSrv.response.error.distance, (long int)errorSrv.response.error.angle);
    // Calculate Twist
    ros::ServiceClient twistClient = n.serviceClient<assignment1::ErrorToTwist>("error_to_twist");
    assignment1::ErrorToTwist twistSrv;
    twistSrv.request.error = errorSrv.response.error;
    if (twistClient.call(twistSrv)) {
      ROS_INFO("Speed: %f", twistSrv.response.twist.linear.x);
    } else {
      ROS_ERROR("Failed to call service ErrorToTwist");
      return;
    }
  }
  else
  {
    ROS_ERROR("Failed to call service SonarsToError");
    return;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");
  Controller controller;
  ros::Subscriber sub = controller.n.subscribe("sonars", 1000, &Controller::sonarsCallback, &controller);
  ros::spin();

  return 0;
}