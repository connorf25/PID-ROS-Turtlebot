#include "ros/ros.h"
#include "assignment1/Sonars.h"
#include "assignment1/GetError.h"

int16_t lastAngle = 0;
assignment1::Sonars lastReading;

void sonarsCallback(const assignment1::Sonars sonars)
{
  lastReading = sonars;
}

bool calcError (
  assignment1::GetError::Request &req,
  assignment1::GetError::Response &res
)
{
  uint16_t min_dist = std::min({lastReading.distance0, lastReading.distance1, lastReading.distance2});
  int16_t angle;
  if (min_dist < 65535) { // Can see bowl with sonars
    if (lastReading.distance0 == min_dist)
    {
      angle = -30;
      lastAngle = -30;
    }
    else if (lastReading.distance1 == min_dist) 
    {
      angle = 0;
      lastAngle = 0;
    }
    else if (lastReading.distance2 == min_dist) 
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
  ros::init(argc, argv, "get_error");
  ros::NodeHandle n;

  // Subscribe to sonars
  ros::Subscriber sub = n.subscribe("sonars", 1000, sonarsCallback);

  ros::ServiceServer service = n.advertiseService("get_error", calcError);
  ros::spin();

  return 0;
}

