#include "UpdaterComponent.h"
#include "ComponentManager.h"

namespace Learning2DEngine
{
	namespace System
	{
		UpdaterComponent::UpdaterComponent(GameObject* gameObject)
			: Component(gameObject)
		{
		}

		void UpdaterComponent::Init()
		{
			ComponentManager::GetInstance().AddToUpdate(this);
		}

		void UpdaterComponent::Destroy()
		{
			ComponentManager::GetInstance().RemoveFromUpdate(this);
		}
	}
}