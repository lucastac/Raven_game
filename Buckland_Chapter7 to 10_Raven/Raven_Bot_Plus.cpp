#include "Raven_Bot_Plus.h"

#include "misc/Cgdi.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_WeaponSystem.h"
#include "Raven_UserOptions.h"

Raven_Bot_Plus::Raven_Bot_Plus(Raven_Game* world, Vector2D pos):
Raven_Bot(world,pos)
{
	m_pBrain = new Goal_Think_Plus(this);
	
}

Raven_Bot_Plus::~Raven_Bot_Plus()
{

}

void Raven_Bot_Plus::Render()                                         
{
  //when a bot is hit by a projectile this value is set to a constant user
  //defined value which dictates how long the bot should have a thick red
  //circle drawn around it (to indicate it's been hit) The circle is drawn
  //as long as this value is positive. (see Render)
  m_iNumUpdatesHitPersistant--;


  if (isDead() || isSpawning()) return;
  
  gdi->BluePen();
  
  m_vecBotVBTrans = WorldTransform(m_vecBotVB,
                                   Pos(),
                                   Facing(),
                                   Facing().Perp(),
                                   Scale());

  gdi->ClosedShape(m_vecBotVBTrans);
  
  //draw the head
  gdi->GreenBrush();
  gdi->Circle(Pos(), 6.0 * Scale().x);


  //render the bot's weapon
  m_pWeaponSys->RenderCurrentWeapon();

  //render a thick red circle if the bot gets hit by a weapon
  if (m_bHit)
  {
    gdi->ThickRedPen();
    gdi->HollowBrush();
    gdi->Circle(m_vPosition, BRadius()+1);

    if (m_iNumUpdatesHitPersistant <= 0)
    {
      m_bHit = false;
    }
  }

  gdi->TransparentText();
  gdi->TextColor(0,255,0);

  if (UserOptions->m_bShowBotIDs)
  {
    gdi->TextAtPos(Pos().x -10, Pos().y-20, ttos(ID()));
  }

  if (UserOptions->m_bShowBotHealth)
  {
    gdi->TextAtPos(Pos().x-40, Pos().y-5, "H:"+ ttos(Health()));
  }

  if (UserOptions->m_bShowScore)
  {
    gdi->TextAtPos(Pos().x-40, Pos().y+10, "Scr:"+ ttos(Score()));
  }

}