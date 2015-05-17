// A human robot interface program by Kosei Demura
// Rospeex is used for voice recognition and speech synthesis by Kosmei Sugiura
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ros/ros.h>
#include <cstdlib>
#include "rospeex_if/rospeex.h"
#include "facilitator_engine/GetQuestion.h"
#include "facilitator_engine/Evaluate.h"
#include "hri.hpp"

int testQuestionService(int id){
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<facilitator_engine::GetQuestion>("GetQuestion");
  facilitator_engine::GetQuestion srv;
  srv.request.id = id;
  srv.request.msg = "dummy";
  if (client.call(srv)) {
    ROS_INFO("Question: %s", srv.response.question.c_str());
    interface.say(srv.response.question.c_str(), "ja", "nict");
  }
  else {
    ROS_ERROR("Failed to call service GetQuestion");
  }
  listen = true;
  sleep(2);
}

bool testEvaluateService(int id){
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<facilitator_engine::Evaluate>("Evaluate");
  facilitator_engine::Evaluate srv;
  srv.request.id = id;

  std::cerr << "child said : " << child_answer << "  size=" << sizeof(child_answer) << std::endl;

  int sum = 0;
  for (int i=0; i< 8; i++) {
    sum += (int) child_answer[i];
  }
  printf("  value=%d \n", sum);

  if (sum == 0) {
    std::cerr << "Input answer: " ;
    std::cin >> child_answer;
  }

  int sum2 = 0;
  for (int i=0; i< sizeof(child_answer); i++) {
    sum2 += (int) child_answer[i];
  }

  child_answer = "value=" + child_answer;
  std::cerr << "Child answer: " << child_answer << std::endl;
  
  if (sum2 == 0) return true;
  srv.request.answer = child_answer; 

  for (int i=0; i< sizeof(child_answer); i++) {
    child_answer[i] = 0;
  }

  //if (boost::regex_match(child_answer,null_message)) return false;
  // 音声認識を優先した場合の処理
  //if (sum == 0)  {
  //  return true;
  //}

  std::string message;
  if (client.call(srv)) {
    ROS_INFO("Confidence: %lf, %s", srv.response.confidence,srv.response.comment.c_str());
    interface.say(srv.response.comment.c_str(), "ja", "nict");
    double value =  srv.response.confidence;
    if (value == 1.0) {
      message = "あたり！頭いいね！";
    }
    else if (0.8 <= value && value < 1.0) {
      message = "すごく惜しいわ！ファイト！";
    } 
    else if (0.6 <= value && value < 0.8) {
      message = "惜しいわ！　頑張ってね。";
    }
    else if (0.4 <= value && value < 0.6) {
      message = "ちょっと違うわ! ハッスル、ハッスル";
    }
    else if (0.2 <= value && value < 0.4) {
      message = "残念！頑張れ";
    }
    else {
      message  = "全然違うわ！一緒に頑張ろう！";
    }
  }
  else {
    ROS_ERROR("Failed to call service Evaluate");
  }
  interface.say(message, "ja", "nict");

  return false;
  ROS_INFO("testEvaluateService: OK"); 
}

void sr_response( const std::string& msg )
{
  using boost::posix_time::ptime;
  using boost::posix_time::second_clock;

  std::cerr << "you said : " << msg << std::endl;
  //boost::regex begin_message("はじめて.*");
  //boost::regex end_message("終わって.*");
  std::string text = "";
  std::string qeustion;

  child_answer = msg;
  //if (listen) { 
    /* if (boost::regex_match(msg, begin_message)) {
      state = 1;
      interface.say("今から掛け算くくの問題を出すよ！", "ja", "nict" );
      listen = false;
    }
    else if (boost::regex_match(msg,end_message)) {
      state = 999;
      interface.say("終わるわ。すごく良かったわよ。", "ja", "nict" );
      listen = true;
    }
    } */
    //child_answer = msg;
  //}
}

void hello_mini()
{
  sleep(1);

#ifdef SHORT_MESSAGE
  std::cerr << message_start_short << std::endl;
  interface.say(message_start_short, "ja", "nict" );
  ros::spinOnce();
#else
  std::cerr << message_start_long << std::endl;
  interface.say(message_start_long, "ja", "nict");    
  ros::spinOnce();
  std::cerr << message_lets_study << std::endl;
  interface.say( message_lets_study , "ja", "nict");   
#endif
  sleep(2);
}


int main(int argc, char** argv)
{
  static int question_no    = 1;
  static int question_max_no = 2;

  ros::init(argc, argv, "hri");
  interface.init();
  interface.registerSRResponse(sr_response);
  interface.setSPIConfig("ja", "nict");
  ros::Rate loop_rate(33); // 33Hz 
  ros::spinOnce();
  
  hello_mini();

  while(ros::ok()) {
    std::cerr << "state:"  << state << std::endl;                                         
    switch (state) {
    case 0:
      break;
    case 1: {
      std::cerr << "state1" << std::endl;
      testQuestionService(question_no);
      state = 2;
      break;
    }
    case 2: {
      std::cerr << "state2" << std::endl;
      int count = 0;
      while (testEvaluateService(question_no)) {
	std::cerr << "count=" << count++ << std::endl;
	usleep(2000*1000);
      }
      usleep(2000*1000);
      
      if (++question_no <= question_max_no) {
	interface.say("じゃあ。もう一問だすよ。", "ja", "nict" );
	state = 1;
      }
      else {
	interface.say("これで終わりにするわね。また、明日遊ぼうね。",  "ja", "nict");
	state = 999;
      }
      break;
    }
    case 999:
      return 0;
      break;
    default:
      break;
    }

    loop_rate.sleep(); 
    ros::spinOnce();
  }
  return 0;
}
