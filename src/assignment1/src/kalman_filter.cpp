#include "ros/ros.h"
#include "assignment1/KalmanFilter.h"

double k = 0;
double p = 0;
double r = 10000;
double y = 0;

bool estimateDistance (
    assignment1::KalmanFilter::Request &req,
    assignment1::KalmanFilter::Response &res
)
{
    double z = req.distance;
    if (y == 0 && p == 0) {
        y = z;
        p = r;
    }
    else {
        k = p / (p + r);
        y = y + k * (z - y);
        p = (1 - k) * p;
    }
    res.distance = y;
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "kalman_filter");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("kalman_filter", estimateDistance);
    ros::spin();

    return 0;
}

