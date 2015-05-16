#include <string>
#include <vector>
#include <math.h>
#include <cmath>

class EvaluatorBase
{
public:
  
  EvaluatorBase() {
  }

  virtual float Evaluate() = 0;
  
};
