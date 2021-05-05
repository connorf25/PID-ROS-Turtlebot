#include "ros/ros.h"
#include "assignment1/Sonars.h"
#include "stdio.h"

double m = 0;
double s = 0;
uint32_t k = 0;

void runningVariance(const assignment1::Sonars sonars)
{
    // Running variance as documented here https://www.johndcook.com/blog/standard_deviation/
    k += 1;
    uint16_t x = std::min({sonars.distance0, sonars.distance1, sonars.distance2});

    if (m == 0) 
    {
        m = x;
    }
    else
    {
        double prevM = m;
        m = prevM + (x - prevM) / k;
        s = s + (x - prevM) * (x - m);
        double variance = s / (k - 1);
        // ROS_INFO("Variance: %f", variance);
        // ROS_INFO("Standard Deviation: %f", sqrt(variance));
        std::cout << x << std::endl;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "calc_variance");
    ros::NodeHandle n;
    // Subscribe to sonars
    ros::Subscriber sub = n.subscribe("sonars", 1000, runningVariance);
    ros::spin();

    return 0;
}