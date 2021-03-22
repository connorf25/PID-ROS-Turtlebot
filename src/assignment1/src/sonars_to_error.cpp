#include "ros/ros.h"
#include "assignment1/SonarsToError.h"

bool calcError (
  assignment1::SonarsToError::Request &req,
  assignment1::SonarsToError::Response &res
)
{
  uint16_t min_dist = std::min({req.sonars.distance0, req.sonars.distance1, req.sonars.distance2});
  int16_t angle;
  if (req.sonars.distance0 == min_dist)
  {
    angle = -30;
  }
  else if (req.sonars.distance1 == min_dist) 
  {
    angle = 0;
  }
  else if (req.sonars.distance2 == min_dist) 
  {
    angle = 30;
  }
  else // Cant see bowl in sonars
  {
      // TODO: Base angle off last seen sonar
    angle = 60;
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

