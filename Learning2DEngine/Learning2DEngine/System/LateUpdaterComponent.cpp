#include "LateUpdaterComponent.h"
#include "ComponentManager.h"

namespace Learning2DEngine
{
	namespace System
	{
		LateUpdaterComponent::LateUpdaterComponent(GameObject* gameObject)
			: Component(gameObject)
		{
		}

		void LateUpdaterComponent::Init()
		{
			ComponentManager::GetInstance().AddToLateUpdate(this);
		}

		void LateUpdaterComponent::Destroy()
		{
			ComponentManager::GetInstance().RemoveFromLateUpdate(this);
		}
	}
}