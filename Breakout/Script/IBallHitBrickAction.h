#pragma once

#include "BrickController.h"

class IBallHitBrickAction
{
public:
	virtual ~IBallHitBrickAction() {}

	virtual void BallHitBrick(BrickController* brick) = 0;
};