#include "ros/ros.h"
#include "my_message/Message.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<my_message::Message>("chatter", 1000);
  ros::Rate loop_rate(10);
  my_message::Message messageObj;
  messageObj.my_message = "Hello World";

  while (ros::ok())
  {
    pub.publish(messageObj);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
