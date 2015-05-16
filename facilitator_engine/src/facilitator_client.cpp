#include "ros/ros.h"
#include "facilitator_engine/GetQuestion.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "facilitator_client");
  if (argc != 3)
  {
    ROS_INFO("usage: facilitator_client id msg");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<facilitator_engine::GetQuestion>("GetQuestion");
  facilitator_engine::GetQuestion srv;
  srv.request.id = atoll(argv[1]);
  srv.request.msg = argv[2];
  if (client.call(srv))
  {
    ROS_INFO("Question: %s", srv.response.question.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service GetQuestion");
    return 1;
  }

  return 0;
}
