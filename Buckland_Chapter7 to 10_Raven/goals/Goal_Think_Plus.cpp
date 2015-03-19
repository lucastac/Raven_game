#include "Goal_Think_Plus.h"
#include <list>
#include "misc/Cgdi.h"
#include "../Raven_ObjectEnumerations.h"
#include "misc/utils.h"
#include "../lua/Raven_Scriptor.h"

#include "Goal_MoveToPosition.h"
#include "Goal_Explore.h"
#include "Goal_GetItem.h"
#include "Goal_Wander.h"
#include "Raven_Goal_Types.h"
#include "Goal_AttackTarget.h"


#include "GetWeaponGoal_Evaluator.h"
#include "GetHealthGoal_Evaluator.h"
#include "ExploreGoal_Evaluator.h"
#include "AttackTargetGoal_Evaluator.h"


Goal_Think_Plus::Goal_Think_Plus(Raven_Bot* pBot):
Goal_Think(pBot)
{
	m_iType = goal_think_plus;


	m_Evaluators.clear();

  double HealthBias = 1.0;
  double ShotgunBias = 0.8;
  double RocketLauncherBias = 0.8;
  double RailgunBias = 0.8;
  double ExploreBias = 0.5;
  double AttackBias = 1.5;

  //create the evaluator objects
  m_Evaluators.push_back(new GetHealthGoal_Evaluator(HealthBias));
  m_Evaluators.push_back(new ExploreGoal_Evaluator(ExploreBias));
  m_Evaluators.push_back(new AttackTargetGoal_Evaluator(AttackBias));
  m_Evaluators.push_back(new GetWeaponGoal_Evaluator(ShotgunBias,
                                                     type_shotgun));
  m_Evaluators.push_back(new GetWeaponGoal_Evaluator(RailgunBias,
                                                     type_rail_gun));
  m_Evaluators.push_back(new GetWeaponGoal_Evaluator(RocketLauncherBias,
                                                     type_rocket_launcher));
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
//  }
	
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

  //assert(MostDesirable && "<Goal_Think_Plus::Arbitrate>: no evaluator selected");
  //m_SubGoals.push_back(new Goal_Explore(m_pOwner));
  MostDesirable->SetGoal(m_pOwner);
 // AddGoal_AttackTarget();
}

void Goal_Think_Plus::AddGoal_Explore()
{
  if (notPresent(goal_explore))
  {
    RemoveAllSubgoals();
    AddSubgoal( new Goal_Explore(m_pOwner));
  }
}

void Goal_Think_Plus::AddGoal_GetItem(unsigned int ItemType)
{
  if (notPresent(ItemTypeToGoalType(ItemType)))
  {
    RemoveAllSubgoals();
    AddSubgoal( new Goal_GetItem(m_pOwner, ItemType));
  }
}

void Goal_Think_Plus::AddGoal_AttackTarget()
{
  if (notPresent(goal_attack_target))
  {
    RemoveAllSubgoals();
    AddSubgoal( new Goal_AttackTarget(m_pOwner));
  }
}