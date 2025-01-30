#pragma once

#include "PowerUpType.h"

class PowerUpActivation
{
public:
	virtual ~PowerUpActivation() {}

	virtual void ActivatePowerUp(PowerUpType powerUp) = 0;
};