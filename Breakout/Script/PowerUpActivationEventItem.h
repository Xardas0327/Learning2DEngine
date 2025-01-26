#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "PowerUpActivation.h"

class PowerUpActivationEventItem final : public virtual Learning2DEngine::EventSystem::EventItem<PowerUpType>
{
private:
	PowerUpActivation* powerUpActivation;
public:
	PowerUpActivationEventItem(PowerUpActivation* powerUpActivation)
		: powerUpActivation(powerUpActivation)
	{

	}

	void Call(PowerUpType powerUpType) override
	{
		powerUpActivation->ActivatePowerUp(powerUpType);
	}
};