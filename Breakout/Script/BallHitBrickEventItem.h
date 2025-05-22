#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IBallHitBrickAction.h"

class BallHitBrickEventItem final : public Learning2DEngine::EventSystem::EventItem<BrickController*>
{
private:
	IBallHitBrickAction* item;
public:
	BallHitBrickEventItem(IBallHitBrickAction* item)
		: item(item)
	{

	}

	void Call(BrickController* brick) override
	{
		item->BallHitBrick(brick);
	}
};