// A human robot interface program by Kosei Demura
// Rospeex is used for voice recognition and speech synthesis by Kosmei Sugiura
#ifndef _HRI_HPP
#define _HRI_HPP

#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ros/ros.h>
#include <cstdlib>
#include "rospeex_if/rospeex.h"
#include "facilitator_engine/GetQuestion.h"
#include "facilitator_engine/Evaluate.h"

#define SHORT_MESSAGE   // テスト用の短い発話メッセージ。長くしたい場合はコメントアウト

static rospeex::Interface interface;
static bool listen = true;
static int state = 1;
static std::string child_answer;

std::string message_start_short = "けんせいこちゃんミニ起動します。よろしくね。"; 
std::string message_start_long  = "けんせいこちゃんミニ起動します。金沢からやってきました。よろしくね。";
std::string message_lets_study  = "ミニと一緒に楽しく勉強しようよ。";

#endif
