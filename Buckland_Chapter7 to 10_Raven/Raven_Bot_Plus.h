#include "Raven_Bot.h"
#include "goals\Goal_Think_Plus.h";

class Raven_Bot_Plus : public Raven_Bot
{
public:

	Raven_Bot_Plus(Raven_Game* world, Vector2D pos);
	virtual ~Raven_Bot_Plus();

	void Render();
};