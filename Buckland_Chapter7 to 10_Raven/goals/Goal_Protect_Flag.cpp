#include "Goal_Protect_Flag.h"
#include "Goal_SeekToPosition.h"
#include "Goal_HuntTarget.h"
#include "Goal_DodgeSideToSide.h"
#include "../Raven_Bot.h"
#include "Goal_MoveToPosition.h"


#include "Messaging/Telegram.h"
#include "../Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "../Raven_Game.h"



//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void Goal_Protect_Flag::Activate()
{
  m_iStatus = active;

  //if this goal is reactivated then there may be some existing subgoals that
  //must be removed
  RemoveAllSubgoals();

  if (m_pOwner->Pos().Distance(StandPos)<=10 &&  m_pOwner->GetTargetSys()->isTargetShootable())
  {
    //if the bot has space to strafe then do so
    Vector2D dummy;
    if (m_pOwner->canStepLeft(dummy) || m_pOwner->canStepRight(dummy))
    {
      AddSubgoal(new Goal_DodgeSideToSide(m_pOwner));
    }

    //if not able to strafe, head directly at the target's position 
    /*else
    {
      AddSubgoal(new Goal_SeekToPosition(m_pOwner, m_pOwner->GetTargetBot()->Pos()));
    }*/
  }

  //if the target is not visible, go hunt it.
  else
  {
	  AddSubgoal(new Goal_MoveToPosition(m_pOwner,StandPos));
  }
}

//-------------------------- Process ------------------------------------------
//-----------------------------------------------------------------------------
int Goal_Protect_Flag::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();
    
  //process the subgoals
  m_iStatus = ProcessSubgoals();

  ReactivateIfFailed();

  return m_iStatus;
}

bool Goal_Protect_Flag::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
		case Msg_YouGotMeYouSOB:
		{
		  std::list<Raven_Bot*> bots = m_pOwner->GetWorld()->GetAllBots();
		  std::list<Raven_Bot*>::const_iterator curBot = bots.begin();
		  for (curBot; curBot != bots.end(); ++curBot)
		  {
			  if((*curBot)->my_type == m_pOwner->my_type)
			  {

				  Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
								  m_pOwner->ID(),
								  (* curBot)->ID(),
								  Msg_ProtectCompleted,
								  NO_ADDITIONAL_INFO);
			  }
		  }

		  m_iStatus = completed;

		  return false;
		}

		case Msg_ProtectCompleted:
		{
			m_iStatus = completed;
			return true;
		}
	}

	return false;
}





