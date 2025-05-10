#pragma once

class IDetectorCustomer
{
public:
	virtual ~IDetectorCustomer() = default;

	virtual void Inform() = 0;
};