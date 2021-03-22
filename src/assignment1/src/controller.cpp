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
  // Not sure if I should add another NodeHandle
  ros::ServiceClient client = n.serviceClient<assignment1::SonarsToError>("sonars_to_error");
  assignment1::SonarsToError srv;
  srv.request.sonars = sonars;
  if (client.call(srv))
  {
    ROS_INFO("Distance: %li, Angle : %li", (long int)srv.response.error.distance, (long int)srv.response.error.angle);
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