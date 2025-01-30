#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "BallHitBrickAction.h"

class BallHitBrickEventItem final : public virtual Learning2DEngine::EventSystem::EventItem<BrickController*>
{
private:
	BallHitBrickAction* item;
public:
	BallHitBrickEventItem(BallHitBrickAction* item)
		: item(item)
	{

	}

	void Call(BrickController* brick) override
	{
		item->BallHitBrick(brick);
	}
};