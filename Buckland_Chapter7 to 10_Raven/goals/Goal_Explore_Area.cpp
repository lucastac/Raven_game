#include "Goal_Explore_Area.h"
#include "../Raven_Bot.h"
#include "../navigation/Raven_PathPlanner.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "Messaging/Telegram.h"
#include "..\Raven_Messages.h"

#include "Goal_SeekToPosition.h"
#include "Goal_FollowPath.h"

#include "Messaging/Telegram.h"
#include "../Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"



//------------------------------ Activate -------------------------------------
//-----------------------------------------------------------------------------
void Goal_Explore_Area::Activate()
{
  m_iStatus = active;

  //if this goal is reactivated then there may be some existing subgoals that
  //must be removed
  RemoveAllSubgoals();

  if (!m_bDestinationIsSet)
  {

	  //grab a random location
	  int Randx = RandInt(-areaRadius, areaRadius);
	  int Randy = RandInt(-areaRadius, areaRadius);

	  Vector2D pos(areaCenter.x + Randx,areaCenter.y + Randy);

    
    m_CurrentDestination = pos;//m_pOwner->GetWorld()->GetMap()->GetRandomNodeLocation();

    m_bDestinationIsSet = true;
  }

  //and request a path to that position
  while(!m_pOwner->GetPathPlanner()->RequestPathToPosition(m_CurrentDestination))
  {
	  int Randx = RandInt(-areaRadius, areaRadius);
	  int Randy = RandInt(-areaRadius, areaRadius);

	  Vector2D pos(areaCenter.x + Randx,areaCenter.y + Randy);

    
	  m_CurrentDestination = pos;
  }

  //the bot may have to wait a few update cycles before a path is calculated
  //so for appearances sake it simple ARRIVES at the destination until a path
  //has been found
  AddSubgoal(new Goal_SeekToPosition(m_pOwner, m_CurrentDestination));
}

//------------------------------ Process -------------------------------------
//-----------------------------------------------------------------------------
int Goal_Explore_Area::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();

  //process the subgoals
  m_iStatus = ProcessSubgoals();

  if(m_iStatus == completed || m_iStatus == failed)
  {
	  Terminate();
  }
  return m_iStatus;
}


void Goal_Explore_Area::Terminate()
{
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
							Msg_quitExploring,
							&areaId);
		}
	}
}

//---------------------------- HandleMessage ----------------------------------
//-----------------------------------------------------------------------------
bool Goal_Explore_Area::HandleMessage(const Telegram& msg)
{
  //first, pass the message down the goal hierarchy
  bool bHandled = ForwardMessageToFrontMostSubgoal(msg);

  //if the msg was not handled, test to see if this goal can handle it
  if (bHandled == false)
  {
    switch(msg.Msg)
    {
    case Msg_PathReady:

      //clear any existing goals
      RemoveAllSubgoals();

      AddSubgoal(new Goal_FollowPath(m_pOwner,
                                     m_pOwner->GetPathPlanner()->GetPath()));

      return true; //msg handled


    case Msg_NoPathAvailable:

      m_iStatus = failed;

      return true; //msg handled

    default: return false;
    }
  }

  //handled by subgoals
  return true;
}




