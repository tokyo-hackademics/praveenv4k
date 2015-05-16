#include "ros/ros.h"
#include "facilitator_engine/GetQuestion.h"

bool getQuestion(facilitator_engine::GetQuestion::Request  &req,
         facilitator_engine::GetQuestion::Response &res)
{
  res.question = "What is the capital of Japan?";
  ROS_INFO("request: x=%ld, y=%s", (long int)req.id, req.msg.c_str());
  ROS_INFO("sending back response: [%s]", res.question.c_str());
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "facilitator_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("GetQuestion", getQuestion);
  ROS_INFO("Facilitator Server Ready.");
  ros::spin();

  return 0;
}
