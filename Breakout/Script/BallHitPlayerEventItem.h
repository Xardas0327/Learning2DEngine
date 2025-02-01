#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IBallHitPlayerAction.h"

class BallHitPlayerEventItem final : public virtual Learning2DEngine::EventSystem::EventItem<>
{
private:
	IBallHitPlayerAction* item;
public:
	BallHitPlayerEventItem(IBallHitPlayerAction* item)
		: item(item)
	{

	}

	void Call() override
	{
		item->BallHitPlayer();
	}
};