#include "ros/ros.h"
#include "facilitator_engine/GetQuestion.h"
#include "facilitator_engine/Evaluate.h"
#include "facilitator_engine/db_access.h"

bool getQuestion(facilitator_engine::GetQuestion::Request  &req,
         facilitator_engine::GetQuestion::Response &res)
{
  QuestionAnswer qa;
  res.question = "What is the capital of Japan?";
  ROS_INFO("request: id=%ld, msg=%s", (long int)req.id, req.msg.c_str());
  ROS_INFO("sending back response: [%s]", res.question.c_str());
  return true;
}

bool evaluate(facilitator_engine::Evaluate::Request  &req,
         facilitator_engine::Evaluate::Response &res)
{
  res.comment = "Correct Answer! Congratulations!";
  res.confidence = 1.0f;
  ROS_INFO("request: id=%ld, answer=%s", (long int)req.id, req.answer.c_str());
  ROS_INFO("sending back response- Confidence : [%lf], Comment: [%s]",res.confidence, res.comment.c_str());
  return true;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "facilitator_server");
  ros::NodeHandle n;

  ros::ServiceServer questionService = n.advertiseService("GetQuestion", getQuestion);
  ros::ServiceServer evaluateService = n.advertiseService("Evaluate", evaluate);
  ROS_INFO("Facilitator Server Ready.");
  ros::spin();

  return 0;
}
