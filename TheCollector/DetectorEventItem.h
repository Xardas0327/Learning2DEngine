#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IDetectorCustomer.h"

class DetectorEventItem final : public Learning2DEngine::EventSystem::EventItem<>
{
private:
	IDetectorCustomer* customer;
public:
	DetectorEventItem(IDetectorCustomer* customer)
		: customer(customer)
	{

	}

	void Call() override
	{
		customer->Inform();
	}
};