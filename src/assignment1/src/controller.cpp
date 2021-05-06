#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "assignment1/Sonars.h"
#include "assignment1/ErrorToTwist.h"
#include "assignment1/GetError.h"
#include "assignment1/KalmanFilter.h"

class Controller {
  public:
    ros::NodeHandle n;
    // Call get error service
    assignment1::Error call_get_error();
    // Calculate twist for publisher
    geometry_msgs::Twist call_error_to_twist(assignment1::Error error);
};

assignment1::Error Controller::call_get_error() {
  // Get sonar readings and calculate Error
  assignment1::Error error;
  ros::ServiceClient errorClient = n.serviceClient<assignment1::GetError>("get_error");
  assignment1::GetError errorSrv;
  if (errorClient.call(errorSrv))
  {
    error = errorSrv.response.error;
    // Kalman filter sonar distance if required
    // #ifdef NOISY_SONAR
    if (error.distance < 65535) {
      ros::ServiceClient kalmanClient = n.serviceClient<assignment1::KalmanFilter>("kalman_filter");
      assignment1::KalmanFilter kalmanSrv;
      kalmanSrv.request.distance = error.distance;
      if (kalmanClient.call(kalmanSrv))
      {
        error.distance = kalmanSrv.response.distance;
      }
      else {
        ROS_ERROR("Failed to call service KalmanFilter");
      }
    }
    // #endif
    return error;
  }
  else
  {
    ROS_ERROR("Failed to call service GetError");
    return error;
  }
}

geometry_msgs::Twist Controller::call_error_to_twist(assignment1::Error error) {
  geometry_msgs::Twist return_twist;
  // Calculate Twist
  ros::ServiceClient twistClient = n.serviceClient<assignment1::ErrorToTwist>("error_to_twist");
  assignment1::ErrorToTwist twistSrv;
  twistSrv.request.error = error;
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
    assignment1::Error error = controller.call_get_error();
    geometry_msgs::Twist twist_command = controller.call_error_to_twist(error);
    // Publish message
    twist_pub.publish(twist_command);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}