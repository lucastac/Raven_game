#ifndef GOAL_EXPLORE_AREA_H
#define GOAL_EXPLORE_AREA_H
#pragma warning (disable:4786)

#include "Goals/Goal_Composite.h"
#include "Raven_Goal_Types.h"


class Raven_Bot;

struct AreaMap{
	Vector2D center;
	double radius;
	int id;
	
	AreaMap(Vector2D a, double b,int c):center(a),radius(b),id(c){}
};



class Goal_Explore_Area : public Goal_Composite<Raven_Bot>
{
private:
  
  Vector2D  m_CurrentDestination;

  //set to true when the destination for the exploration has been established
  bool      m_bDestinationIsSet;

public:
	Vector2D areaCenter;
	double areaRadius;
	int areaId;

	Goal_Explore_Area(Raven_Bot* pOwner,AreaMap a):Goal_Composite<Raven_Bot>(pOwner,
                                                            goal_explore_area),
                                  m_bDestinationIsSet(false)
  {
	areaCenter = a.center;
	areaRadius = a.radius;
	areaId = a.id;

  }


  void Activate();

  int Process();

  void Terminate();

  bool HandleMessage(const Telegram& msg);
};





#endif
