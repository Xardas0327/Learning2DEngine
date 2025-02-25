#pragma once

#include "PowerUpType.h"

class IPowerUpActivation
{
public:
	virtual ~IPowerUpActivation() {}

	virtual void ActivatePowerUp(PowerUpType powerUp, bool isActive) = 0;
};