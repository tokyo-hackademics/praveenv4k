#include <string>
#include <vector>
#include "facilitator_engine/math_evaluator.h"

class GeographyEvaluator: EvaluatorBase
{
public:
  
  GeographyEvaluator(std::string actualAnswer, std::string receivedAnswer):_actualAnswer(actualAnswer),_receivedAnswer(receivedAnswer) {
  }

  virtual float Evaluate(){
    //float range = _actualAnswer;
    //float confidence = (1-(fabs(_actualAnswer-_receivedAnswer)/range))
    //return confidence;

//TODO Use Geobytes API followed by Mathevaluator 
    return 0;
  }

private:
 std::string _actualAnswer;
 std::string _receivedAnswer;
  
};
