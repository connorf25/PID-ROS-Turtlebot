#include "ros/ros.h"
#include "assignment1/KalmanFilter.h"
#include "geometry_msgs/Point.h"
#include "gazebo_msgs/GetModelState.h"

geometry_msgs::Point prev_position, current_position;
double p = 100; // This is arbitarily set as positional data should have no variance
double r = 10000; // Experimentally determined that the sonar variance is ~10000
double k = p / (p + r); // This k value will always stay constant
double y = 0;
bool initialize = true; // Boolean to store if this is first loop

double euclideanDistance(geometry_msgs::Point prev, geometry_msgs::Point current) {
    return sqrt(pow(prev.x - current.x, 2) + pow(prev.y - current.y, 2));
}

geometry_msgs::Point getPosition() {
    ros::NodeHandle n;
    gazebo_msgs::GetModelState srv;
    ros::ServiceClient locations = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
    if (!locations.call(srv)) {
        ROS_ERROR("Failed to query model state. Is gazebo running?");
    }
    return srv.response.pose.position;
}

bool estimateDistance (
    assignment1::KalmanFilter::Request &req,
    assignment1::KalmanFilter::Response &res
)
{
    // Initialize
    double z = req.distance;
    current_position = getPosition();

    if (initialize) {
        initialize = false;
        prev_position = current_position;
        y = z;
    }

    // Prediction
    // This assumes that the robot is always directly towards the bowl when it "sees" it
    y = y - euclideanDistance(prev_position, current_position); 
    p = p; // This will be the same each step as value is being read from gazebo

    // Correction
    k = p / (p + r);
    y = y + k * (z - y);
    // p = (1 - k) * p; // This line is not used as the p value is constant

    prev_position = current_position;
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

