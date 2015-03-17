#pragma once
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"

class Goal_Think_Plus :	public Goal_Think
{
public:
	Goal_Think_Plus(Raven_Bot* pBot);
  ~Goal_Think_Plus();

  void Activate();
  int Process();
  void Arbitrate();
};

