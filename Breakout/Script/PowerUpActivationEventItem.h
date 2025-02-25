#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IPowerUpActivation.h"

class PowerUpActivationEventItem final : public virtual Learning2DEngine::EventSystem::EventItem<PowerUpType, bool>
{
private:
	IPowerUpActivation* powerUpActivation;
public:
	PowerUpActivationEventItem(IPowerUpActivation* powerUpActivation)
		: powerUpActivation(powerUpActivation)
	{

	}

	void Call(PowerUpType powerUpType, bool isActive) override
	{
		powerUpActivation->ActivatePowerUp(powerUpType, isActive);
	}
};