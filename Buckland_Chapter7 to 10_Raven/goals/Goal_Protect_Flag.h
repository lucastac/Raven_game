#ifndef GOAL_PROTECT_FLAG_H
#define GOAL_PROTECT_FLAG_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_AttackTarget.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "Goals/Goal_Composite.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"





class Goal_Protect_Flag : public Goal_Composite<Raven_Bot>
{

private:
	Vector2D  StandPos;

public:

  Goal_Protect_Flag(Raven_Bot* pOwner,Vector2D pos):Goal_Composite<Raven_Bot>(pOwner, goal_protect_flag)
  {
	  StandPos = pos;

  }

  void Activate();

  int  Process();

  void Terminate(){m_iStatus = completed;}

  bool HandleMessage(const Telegram& msg);

};






#endif
