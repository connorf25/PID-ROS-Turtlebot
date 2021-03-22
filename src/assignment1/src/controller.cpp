#include "ros/ros.h"
#include "assignment1/Sonars.h"
#include "assignment1/SonarsToError.h"

class Controller {
  public:
    ros::NodeHandle n;
    void sonarsCallback(const assignment1::Sonars);
};

void Controller::sonarsCallback(const assignment1::Sonars sonars)
{
  ros::ServiceClient errorClient = n.serviceClient<assignment1::SonarsToError>("sonars_to_error");
  assignment1::SonarsToError errorSrv;
  errorSrv.request.sonars = sonars;
  if (errorClient.call(errorSrv))
  {
    ROS_INFO("Distance: %li, Angle : %li", (long int)errorSrv.response.error.distance, (long int)errorSrv.response.error.angle);
  }
  else
  {
    ROS_ERROR("Failed to call service/s");
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