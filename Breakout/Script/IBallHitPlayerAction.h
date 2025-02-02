#pragma once

class IBallHitPlayerAction
{
public:
	virtual ~IBallHitPlayerAction() {}

	virtual void BallHitPlayer() = 0;
};