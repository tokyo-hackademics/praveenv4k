#include "ros/ros.h"
#include "facilitator_engine/GetQuestion.h"
#include "facilitator_engine/Evaluate.h"
#include "facilitator_engine/db_access.h"
#include "database_interface/postgresql_database.h"
#include "facilitator_engine/geography_evaluator.h"

#define MAX_BUF_SIZE 256

bool getQuestion(facilitator_engine::GetQuestion::Request  &req,
         facilitator_engine::GetQuestion::Response &res)
{
  ROS_INFO("request: id=%ld, msg=%s", (long int)req.id, req.msg.c_str());

  database_interface::PostgresqlDatabase 
    database("localhost", "5432",
             "postgres", "postgres", "question_answer");
  if (!database.isConnected())
  {
    std::cerr << "Database failed to connect \n";
    return -1;
  }
  std::cerr << "Database connected successfully \n";
  
  std::vector< boost::shared_ptr<QuestionAnswer> > qa;
  char buf[MAX_BUF_SIZE];
  sprintf (buf, "id=%d", (int)req.id);
  std::string where_clause(buf);
  database.getList(qa, where_clause);

  if(qa.size()>0){
    qa[0]->question_.toString(res.question);
    ROS_INFO("Item Found - Sending back response: [%s]", res.question.c_str());
    return true;
  }
  return false;
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

// CHECK_DIST API
// http://getdistance.geobytes.com/GetDistance?callback=callback&location1=USNYNYOR&location2=USMTBABB 
// GET GEO LOCATION API
// http://getcitydetails.geobytes.com/GetCityDetails?fqcn=osaka

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
