#include "ros/ros.h"
#include "assignment1/SonarsToError.h"

int16_t lastAngle = 0;

bool calcError (
  assignment1::SonarsToError::Request &req,
  assignment1::SonarsToError::Response &res
)
{
  uint16_t min_dist = std::min({req.sonars.distance0, req.sonars.distance1, req.sonars.distance2});
  int16_t angle;
  if (min_dist < 65535) { // Can see bowl with sonars
    if (req.sonars.distance0 == min_dist)
    {
      angle = -30;
      lastAngle = -30;
    }
    else if (req.sonars.distance1 == min_dist) 
    {
      angle = 0;
      lastAngle = 0;
    }
    else if (req.sonars.distance2 == min_dist) 
    {
      angle = 30;
      lastAngle = 30;
    }
    else {
      ROS_ERROR("This should not be hit");
    }
  }
  else // Cant see bowl in sonars
  {
    if (lastAngle > 0)
      angle = 60;
    else
      angle = -60;
  }
  res.error.angle = angle;
  res.error.distance = min_dist;
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonars_to_error");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("sonars_to_error", calcError);
  ros::spin();

  return 0;
}

