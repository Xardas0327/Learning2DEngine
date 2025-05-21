#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IPowerUpActivation.h"

class PowerUpActivationEventItem final : public Learning2DEngine::EventSystem::EventItem<PowerUpType>
{
private:
	IPowerUpActivation* powerUpActivation;
public:
	PowerUpActivationEventItem(IPowerUpActivation* powerUpActivation)
		: powerUpActivation(powerUpActivation)
	{

	}

	void Call(PowerUpType powerUpType) override
	{
		powerUpActivation->ActivatePowerUp(powerUpType);
	}
};