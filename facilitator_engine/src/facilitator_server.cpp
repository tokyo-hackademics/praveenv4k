#include "ros/ros.h"
#include "facilitator_engine/GetQuestion.h"
#include "facilitator_engine/Evaluate.h"
#include "facilitator_engine/db_access.h"
#include "database_interface/postgresql_database.h"
#include "facilitator_engine/geography_evaluator.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

//#include <boost/algorithm/string/replace.hpp>

using boost::lexical_cast;
using boost::bad_lexical_cast;
using namespace std;

#define MAX_BUF_SIZE 256

bool getDouble(std::string& str, double& val){
  bool ret = true;
  try
  {
    val = lexical_cast<double>(str);
  }
  catch(const bad_lexical_cast &)
  {
    ret = false;
  }
  return ret;
}

bool areEqual(std::string str1, std::string str2){
  return boost::iequals(str1, str2);
}

bool retrieveFromDb(int id, boost::shared_ptr<QuestionAnswer>& qa){
  database_interface::PostgresqlDatabase 
    database("localhost", "5432",
             "postgres", "postgres", "question_answer");
  if (!database.isConnected())
  {
    std::cerr << "Database failed to connect \n";
    return -1;
  }
  std::cerr << "Database connected successfully \n";
  
  std::vector< boost::shared_ptr<QuestionAnswer> > qaList;
  char buf[MAX_BUF_SIZE];
  sprintf (buf, "id=%d", (int)id);
  std::string where_clause(buf);
  database.getList(qaList, where_clause);

  if(qaList.size()>0){
    qa = qaList[0];
    return true;
  }
  return false;
}

bool getQuestion(facilitator_engine::GetQuestion::Request  &req,
         facilitator_engine::GetQuestion::Response &res)
{
  ROS_INFO("GET Request: id=%ld, msg=%s", (long int)req.id, req.msg.c_str());
  boost::shared_ptr<QuestionAnswer> qa;
  if(retrieveFromDb((int)req.id,qa) && qa != 0){
    qa->question_.toString(res.question);
    ROS_INFO("Item Found - Sending back response: [%s]", res.question.c_str());
    return true;
  }else{
    res.question = "Requestion question is not found !";
  }
  return false;
}

bool evaluate(facilitator_engine::Evaluate::Request  &req,
         facilitator_engine::Evaluate::Response &res)
{
  ROS_INFO("Evaluate Request: id=%ld, answer=%s", (long int)req.id, req.answer.c_str());
  boost::shared_ptr<QuestionAnswer> qa;
  if(retrieveFromDb((int)req.id,qa) && qa != 0){
    std::string method;
    qa->evaluation_type_.toString(method);
    if(areEqual(method,"arithmetic")){
	double actual, received;
        std::string actualStr;
        qa->answer_.toString(actualStr);
        //const std::string input = actualStr;
        //std::string output = boost::replace_all_copy(input, "#", "@"); 
	std::string receivedStr=req.answer;
        boost::replace_all(receivedStr, "value=", "0");
        ROS_INFO("%s, %s", req.answer.c_str(), actualStr.c_str());
        if(getDouble(receivedStr,received)&&getDouble(actualStr,actual))
	{
          ROS_INFO("Conversion successful: %lf, %lf!",actual,received);
	  MathEvaluator m(actual,received);
          res.confidence = m.Evaluate();
	     return true;
	}
    }
  } 
  res.comment = "Evaluation method not found!";
  res.confidence = 0.0f;
  return false;
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
