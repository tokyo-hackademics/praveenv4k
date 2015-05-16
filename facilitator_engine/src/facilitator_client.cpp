#include "ros/ros.h"
#include "facilitator_engine/GetQuestion.h"
#include "facilitator_engine/Evaluate.h"
#include <cstdlib>

int testQuestionService(char **argv){
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
  }
}

int testEvaluateService(int id, const char* answer){
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<facilitator_engine::Evaluate>("Evaluate");
  facilitator_engine::Evaluate srv;
  srv.request.id = id;
  srv.request.answer = answer;
  if (client.call(srv))
  {
    ROS_INFO("Confidence: %lf, %s", srv.response.confidence,srv.response.comment.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Evaluate");
  }
  ROS_INFO("testEvaluateService: OK");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "facilitator_client");
  if (argc != 3)
  {
    ROS_INFO("usage: facilitator_client id msg");
    return 1;
  }

  testQuestionService(argv);
  testEvaluateService(1,"Answer is 42!");

  return 0;
}
