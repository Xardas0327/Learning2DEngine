#include "Component.h"

#include "GameObject.h"

namespace Learning2DEngine
{
	namespace System
	{
		Component::Component(GameObject* gameObject)
			: gameObject(gameObject), isActive(true)
		{

		}

		void Component::Init()
		{

		}

		void Component::Destroy()
		{

		}

		void Component::SetActive(bool active)
		{
			isActive = active;
		}

		bool Component::IsActive() const
		{
			return isActive && gameObject->IsActive();
		}
	}
}