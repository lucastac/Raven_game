#include "ProtectFlagGoal_Evaluator.h"
#include "../Raven_ObjectEnumerations.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../navigation/Raven_PathPlanner.h"
#include "../Raven_Game.h"


//---------------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double ProtectFlagGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	int type_flag = 13 + pBot->my_type;

	Raven_PathPlanner* pathplanner = pBot->GetPathPlanner();
	int nodeItem = pathplanner->GetPosToClosestItem(type_flag);
	if(nodeItem == -1)return 0;
	Vector2D posItem = pBot->GetWorld()->GetMap()->GetNavGraph().GetNode(nodeItem).Pos();


	double distEnemy = 0;
	double MindistEnemy = 1000;
	double distAlly = 0;

	std::list<Raven_Bot*> bots = pBot->GetWorld()->GetAllBots();
	std::list<Raven_Bot*>::const_iterator curBot = bots.begin();
	for (curBot; curBot != bots.end(); ++curBot)
	{
		if((*curBot)->my_type == pBot->my_type)
		{
			distAlly += posItem.Distance((*curBot)->Pos());
		}else
		{
			double dist = posItem.Distance((*curBot)->Pos());
			distEnemy += dist;

			if(dist<MindistEnemy)
				MindistEnemy = dist;
		}
	}


	double MyDistance = posItem.Distance(pBot->Pos());

	distAlly -= MyDistance;

	double AverageAlly = distAlly/(bots.size()/2 -1);
	double AverageEnemy = distEnemy/(bots.size()/2);

	//se os inimigos estão muito longe não é necessário proteger a base
	if(MindistEnemy>=500) return 0;

    //value used to tweak the desirability
    const double Tweaker = 0.2;

	double Desirability = Tweaker * AverageAlly / 
									(MyDistance);
 
    //ensure the value is in the range 0 to 1
    Clamp(Desirability, 0, 1);
  
    //bias the value according to the personality of the bot
    Desirability *= m_dCharacterBias;

    return Desirability;
}



//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void ProtectFlagGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
  pBot->GetBrain()->AddGoal_Protect_Flag(); 
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void ProtectFlagGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	int type_flag = 14 - pBot->my_type;
  gdi->TextAtPos(Position, "H: " + ttos(CalculateDesirability(pBot), 2));
  return;
  
  std::string s = ttos(1-Raven_Feature::Health(pBot)) + ", " + ttos(Raven_Feature::DistanceToItem(pBot, type_flag));
  gdi->TextAtPos(Position+Vector2D(0,15), s);
}