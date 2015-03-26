#ifndef RAVEN_FLAG_EVALUATOR
#define RAVEN_FLAG_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GetFlagGoal_Evaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to calculate how desirable the goal of fetching a flag item
//          is
//-----------------------------------------------------------------------------

#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"

class GetFlagGoal_Evaluator : public Goal_Evaluator
{
public:

  GetFlagGoal_Evaluator(double bias):Goal_Evaluator(bias){}
  
  double CalculateDesirability(Raven_Bot* pBot);

  void  SetGoal(Raven_Bot* pEnt);

  void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};

#endif
    
