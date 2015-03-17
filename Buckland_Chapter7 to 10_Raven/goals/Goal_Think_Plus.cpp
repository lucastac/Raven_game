#include "Goal_Think_Plus.h"


Goal_Think_Plus::Goal_Think_Plus(Raven_Bot* pBot):
Goal_Think(pBot)
{
	m_iType = goal_think_plus;
}


Goal_Think_Plus::~Goal_Think_Plus(void)
{
  GoalEvaluators::iterator curDes = m_Evaluators.begin();
  for (curDes; curDes != m_Evaluators.end(); ++curDes)
  {
    delete *curDes;
  }
}


void Goal_Think_Plus::Activate()
{
  if (!m_pOwner->isPossessed())
  {
    Arbitrate();
  }

  m_iStatus = active;
}

//------------------------------ Process --------------------------------------
//
//  processes the subgoals
//-----------------------------------------------------------------------------
int Goal_Think_Plus::Process()
{
  ActivateIfInactive();
  
  int SubgoalStatus = ProcessSubgoals();

  //if (SubgoalStatus == completed || SubgoalStatus == failed)
  //{
    if (!m_pOwner->isPossessed())
    {
      m_iStatus = inactive;
	 // m_SubGoals.clear();
    }
  //}

  return m_iStatus;
}

//----------------------------- Update ----------------------------------------
// 
//  this method iterates through each goal option to determine which one has
//  the highest desirability.
//-----------------------------------------------------------------------------
void Goal_Think_Plus::Arbitrate()
{
  double best = 0;
  Goal_Evaluator* MostDesirable = 0;

  //iterate through all the evaluators to see which produces the highest score
  GoalEvaluators::iterator curDes = m_Evaluators.begin();
  for (curDes; curDes != m_Evaluators.end(); ++curDes)
  {
    double desirabilty = (*curDes)->CalculateDesirability(m_pOwner);

    if (desirabilty >= best)
    {
      best = desirabilty;
      MostDesirable = *curDes;
    }
  }

  assert(MostDesirable && "<Goal_Think::Arbitrate>: no evaluator selected");

  MostDesirable->SetGoal(m_pOwner);
}