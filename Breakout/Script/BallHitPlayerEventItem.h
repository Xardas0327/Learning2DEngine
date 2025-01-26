#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "BallHitPlayerAction.h"

class BallHitPlayerEventItem final : public virtual Learning2DEngine::EventSystem::EventItem<>
{
private:
	BallHitPlayerAction* item;
public:
	BallHitPlayerEventItem(BallHitPlayerAction* item)
		: item(item)
	{

	}

	void Call() override
	{
		item->BallHitPlayer();
	}
};