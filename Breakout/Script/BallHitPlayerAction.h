#pragma once

class BallHitPlayerAction
{
public:
	virtual ~BallHitPlayerAction() {}

	virtual void BallHitPlayer() = 0;
};