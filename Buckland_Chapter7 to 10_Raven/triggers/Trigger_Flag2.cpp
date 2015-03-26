#include "Trigger_Flag2.h"
#include "misc/Cgdi.h"
#include "misc/Stream_Utility_Functions.h"
#include <fstream>
#include "../lua/Raven_Scriptor.h"
#include "../constants.h"
#include "../Raven_ObjectEnumerations.h"


///////////////////////////////////////////////////////////////////////////////
Trigger_Flag2::Trigger_Flag2(std::ifstream& datafile):
      
     Trigger_Respawning<Raven_Bot>(GetValueFromStream<int>(datafile))
{
  Read(datafile);
}


void Trigger_Flag2::Try(Raven_Bot* pBot)
{
  if (isActive() && isTouchingTrigger(pBot->Pos(), pBot->BRadius()))
  {
    pBot->IncreaseHealth(m_iHealthGiven);

    Deactivate();
  } 
}


void Trigger_Flag2::Render()
{
  if (isActive())
  {
    const int sz = 5;
    gdi->BlackPen();

	gdi->Line(Pos().x, Pos().y-2*sz, Pos().x, Pos().y+2*sz+1);
    gdi->GreenBrush();
    
    gdi->Rect(Pos().x, Pos().y, Pos().x-2*sz-1, Pos().y-2*sz-1);
    //gdi->RedPen();
   // gdi->Line(Pos().x, Pos().y-sz, Pos().x, Pos().y+sz+1);
    //gdi->Line(Pos().x-sz, Pos().y, Pos().x+sz+1, Pos().y);
  }
}


void Trigger_Flag2::Read(std::ifstream& in)
{
  double x, y, r;
  int GraphNodeIndex;
  
  in >> x >> y  >> r >> m_iHealthGiven >> GraphNodeIndex;

  SetPos(Vector2D(x,y)); 
  SetBRadius(r);
  SetGraphNodeIndex(GraphNodeIndex);

  //create this trigger's region of fluence
  AddCircularTriggerRegion(Pos(), script->GetDouble("DefaultGiverTriggerRange"));

  SetRespawnDelay((unsigned int)(script->GetDouble("Flag_RespawnDelay") * FrameRate));
  SetEntityType(type_flag2);
}
