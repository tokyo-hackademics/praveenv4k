#include <string>
#include <vector>
#include "facilitator_engine/evaluator_base.h"

class MathEvaluator: EvaluatorBase
{
public:
  
  MathEvaluator(double actualAnswer, double receivedAnswer):_actualAnswer(actualAnswer),_receivedAnswer(receivedAnswer) {
  }

  virtual float Evaluate(){
    float range = fabs(_actualAnswer);
    if(range < 1e-6){
      range = 2*fabs(_receivedAnswer);
    }
    float confidence = (1-(fabs(_actualAnswer-_receivedAnswer)/range));
    return confidence;
  }

private:
 double _actualAnswer;
 double _receivedAnswer;
  
};
