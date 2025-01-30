#pragma once

#include "BrickController.h"

class BallHitBrickAction
{
public:
	virtual ~BallHitBrickAction() {}

	virtual void BallHitBrick(BrickController* brick) = 0;
};