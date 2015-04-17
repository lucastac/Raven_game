#include "Goal_Think.h"
#include <list>
#include "misc/Cgdi.h"
#include "../Raven_ObjectEnumerations.h"
#include "misc/utils.h"
#include "../lua/Raven_Scriptor.h"

#include "../Raven_Game.h"

#include "Messaging/Telegram.h"
#include "../Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "Goal_MoveToPosition.h"
#include "Goal_Explore.h"
#include "Goal_GetItem.h"
#include "Goal_Wander.h"
#include "Raven_Goal_Types.h"
#include "Goal_AttackTarget.h"
#include "Goal_Protect_Flag.h"


#include "GetWeaponGoal_Evaluator.h"
#include "GetHealthGoal_Evaluator.h"
#include "ExploreGoal_Evaluator.h"
#include "AttackTargetGoal_Evaluator.h"
#include "GetFlagGoal_Evaluator.h"
#include "ExploreAreaGoal_Evaluator.h"
#include "ProtectFlagGoal_Evaluator.h"

//#include "Goal_Think_Plus.h"


Goal_Think::Goal_Think(Raven_Bot* pBot):Goal_Composite<Raven_Bot>(pBot, goal_think)
{
  
  //these biases could be loaded in from a script on a per bot basis
  //but for now we'll just give them some random values
 /* const double LowRangeOfBias = 0.5;
  const double HighRangeOfBias = 1.5;

  double HealthBias = RandInRange(LowRangeOfBias, HighRangeOfBias);
  double ShotgunBias = RandInRange(LowRangeOfBias, HighRangeOfBias);
  double RocketLauncherBias = RandInRange(LowRangeOfBias, HighRangeOfBias);
  double RailgunBias = RandInRange(LowRangeOfBias, HighRangeOfBias);
  double ExploreBias = RandInRange(LowRangeOfBias, HighRangeOfBias);
  double AttackBias = RandInRange(LowRangeOfBias, HighRangeOfBias);

  //create the evaluator objects
  m_Evaluators.push_back(new GetHealthGoal_Evaluator(HealthBias));
  m_Evaluators.push_back(new ExploreGoal_Evaluator(ExploreBias));
  m_Evaluators.push_back(new AttackTargetGoal_Evaluator(AttackBias));
  m_Evaluators.push_back(new GetWeaponGoal_Evaluator(ShotgunBias,
                                                     type_shotgun));
  m_Evaluators.push_back(new GetWeaponGoal_Evaluator(RailgunBias,
                                                     type_rail_gun));
  m_Evaluators.push_back(new GetWeaponGoal_Evaluator(RocketLauncherBias,
                                                     type_rocket_launcher));*/


  double HealthBias = 1.0;
  double ShotgunBias = 0.8;
  double RocketLauncherBias = 0.8;
  double RailgunBias = 0.8;
  double ExploreBias = 10.0;
  double AttackBias = 6.0;
  double FlagBias = 1.5;
  double ProcFlagBias = 2.0;

  //create the evaluator objects
  //m_Evaluators.push_back(new GetHealthGoal_Evaluator(HealthBias));
  m_Evaluators.push_back(new ExploreAreaGoal_Evaluator(ExploreBias));
  m_Evaluators.push_back(new AttackTargetGoal_Evaluator(AttackBias));
 // m_Evaluators.push_back(new GetWeaponGoal_Evaluator(ShotgunBias,
 //                                                    type_shotgun));
 // m_Evaluators.push_back(new GetWeaponGoal_Evaluator(RailgunBias,
 //                                                    type_rail_gun));
 // m_Evaluators.push_back(new GetWeaponGoal_Evaluator(RocketLauncherBias,
 //                                                    type_rocket_launcher));
  m_Evaluators.push_back(new GetFlagGoal_Evaluator(FlagBias));

  m_Evaluators.push_back(new ProtectFlagGoal_Evaluator(ProcFlagBias));


	  //quatro quadrantes centrais
  	  m_Areas.push_back(AreaMap(Vector2D(350,150),200,0));
	  m_Areas.push_back(AreaMap(Vector2D(650,150),200,1));
	  m_Areas.push_back(AreaMap(Vector2D(350,350),200,2));
	  m_Areas.push_back(AreaMap(Vector2D(650,350),200,3));
	  //quandrante da bandeira marrom
	  m_Areas.push_back(AreaMap(Vector2D(10,250),80,4));
	  //quadranta da bandeira verde
	  m_Areas.push_back(AreaMap(Vector2D(940,250),80,5));

}

//----------------------------- dtor ------------------------------------------
//-----------------------------------------------------------------------------
Goal_Think::~Goal_Think()
{
  GoalEvaluators::iterator curDes = m_Evaluators.begin();
  for (curDes; curDes != m_Evaluators.end(); ++curDes)
  {
    delete *curDes;
  }
}

//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void Goal_Think::Activate()
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
int Goal_Think::Process()
{
	if(m_iStatus != completed)
	{
	  //if(m_pOwner->my_type ==1) return active ;
	  ActivateIfInactive();
  
	  int SubgoalStatus = ProcessSubgoals();

	  if (SubgoalStatus == completed || SubgoalStatus == failed)
	  {
		if (!m_pOwner->isPossessed())
		{
		  m_iStatus = inactive;
		}
	  }
	}

  return m_iStatus;
}

//----------------------------- Update ----------------------------------------
// 
//  this method iterates through each goal option to determine which one has
//  the highest desirability.
//-----------------------------------------------------------------------------
void Goal_Think::Arbitrate()
{
	if(m_iStatus == completed) return;
	//if(m_pOwner->my_type ==1) return ;
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

  //assert(MostDesirable && "<Goal_Think::Arbitrate>: no evaluator selected");

  MostDesirable->SetGoal(m_pOwner);
 // AddGoal_Explore_Area();
  //AddGoal_GetItem(14 - m_pOwner->my_type);
}


//---------------------------- notPresent --------------------------------------
//
//  returns true if the goal type passed as a parameter is the same as this
//  goal or any of its subgoals
//-----------------------------------------------------------------------------
bool Goal_Think::notPresent(unsigned int GoalType)const
{
  if (!m_SubGoals.empty())
  {
    return m_SubGoals.front()->GetType() != GoalType;
  }

  return true;
}

void Goal_Think::AddGoal_MoveToPosition(Vector2D pos)
{
  AddSubgoal( new Goal_MoveToPosition(m_pOwner, pos));
}

void Goal_Think::AddGoal_Protect_Flag(Vector2D StandPos)
{
	//int ar = 4 + m_pOwner->my_type;
	if(notPresent(goal_protect_flag))
	{
		RemoveAllSubgoals();
		//AddSubgoal( new Goal_Explore_Area(m_pOwner,m_Areas[ar]) );
		AddSubgoal(new Goal_Protect_Flag(m_pOwner,StandPos));
	}/*else if( ( (Goal_Explore_Area*)m_SubGoals.front())->areaId != ar )
	{
		RemoveAllSubgoals();
		AddSubgoal( new Goal_Explore_Area(m_pOwner,m_Areas[ar]) );
	}*/
}

void Goal_Think::AddGoal_Explore_Area()
{
  if (notPresent(goal_explore_area))
  {
	  bool teste = true;
	  int id = 0;

	  do{
		  teste = true;

		   id = RandInt(0,m_Areas.size()-1);
	  
		  for(int i =0;i<m_AreasExploring.size();i++)
		  {
			  if(m_AreasExploring[i] == id)
			  {
				  teste = false;
				  break;
			  }
		  }
	  }while(!teste);

    RemoveAllSubgoals();

	AddSubgoal( new Goal_Explore_Area(m_pOwner,m_Areas[id]));

		  Raven_Game * world = m_pOwner->GetWorld();

		  std::list<Raven_Bot*> bots = world->GetAllBots();
		  std::list<Raven_Bot*>::const_iterator curBot = bots.begin();
		  for (curBot; curBot != bots.end(); ++curBot)
		  {
			  if((*curBot)->my_type == m_pOwner->my_type)
			  {
				  Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
								  m_pOwner->ID(),
								  (* curBot)->ID(),
								  Msg_Exploring,
								  &m_Areas[id].id);
			  }
		  }

  }
}

void Goal_Think::AddGoal_Explore()
{
  if (notPresent(goal_explore))
  {
    RemoveAllSubgoals();
    AddSubgoal( new Goal_Explore(m_pOwner));
  }
}

void Goal_Think::AddGoal_GetItem(unsigned int ItemType)
{  if (notPresent(ItemTypeToGoalType(ItemType)))
  {
    RemoveAllSubgoals();
    AddSubgoal( new Goal_GetItem(m_pOwner, ItemType));
  }
}

void Goal_Think::AddGoal_AttackTarget()
{
  if (notPresent(goal_attack_target))
  {
    RemoveAllSubgoals();
    AddSubgoal( new Goal_AttackTarget(m_pOwner));
  }
}

//-------------------------- Queue Goals --------------------------------------
//-----------------------------------------------------------------------------
void Goal_Think::QueueGoal_MoveToPosition(Vector2D pos)
{
   m_SubGoals.push_back(new Goal_MoveToPosition(m_pOwner, pos));
}



//----------------------- RenderEvaluations -----------------------------------
//-----------------------------------------------------------------------------
void Goal_Think::RenderEvaluations(int left, int top)const
{
  gdi->TextColor(Cgdi::black);
  
  std::vector<Goal_Evaluator*>::const_iterator curDes = m_Evaluators.begin();
  for (curDes; curDes != m_Evaluators.end(); ++curDes)
  {
    (*curDes)->RenderInfo(Vector2D(left, top), m_pOwner);

    left += 75;
  }
}

void Goal_Think::Render()
{
  std::list<Goal<Raven_Bot>*>::iterator curG;
  for (curG=m_SubGoals.begin(); curG != m_SubGoals.end(); ++curG)
  {
    (*curG)->Render();
  }
}

bool Goal_Think::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
		case Msg_Exploring:
		{
			int ar = (*(int*)msg.ExtraInfo);
			bool alreadyKnown = false;

			for(int i =0;i<m_AreasExploring.size();i++)
			{
				if(m_AreasExploring[i] == ar)
				{
					alreadyKnown = true;
					break;
				}
			}

			if(!alreadyKnown)
			{
				m_AreasExploring.push_back(ar);
			}

			return true;
		}

		case Msg_quitExploring:
		{
			int ar = (*(int*)msg.ExtraInfo);
			for(int i =0;i<m_AreasExploring.size();i++)
			{
				if(m_AreasExploring[i] == ar)
				{
					m_AreasExploring[i]= m_AreasExploring[m_AreasExploring.size()-1];
					m_AreasExploring.pop_back();
					break;
				}
			}

			return true;
		}

		case Msg_TakeThatMF:
		{
			if(m_pOwner->Health() <=100 && m_iStatus != completed) m_iStatus = inactive;
			return ForwardMessageToFrontMostSubgoal(msg);
		}

		case Msg_We_Won:
		{
			RemoveAllSubgoals();
			m_iStatus = completed;
			//if(msg.Sender == m_pOwner->my_type) AddGoal_Explore();
			//else AddGoal_AttackTarget();
		}

		default: return ForwardMessageToFrontMostSubgoal(msg);
	}
	
}


   
