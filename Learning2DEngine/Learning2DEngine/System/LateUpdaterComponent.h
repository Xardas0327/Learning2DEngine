#pragma once

#include "BaseLateUpdaterComponent.h"
#include "ObjectManager.h"

namespace Learning2DEngine
{
	namespace System
	{
		/// <summary>
		/// The classes, which are inherited from UpdaterComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about `Component` and `BaseLateUpdaterComponent`.
		/// </summary>
		class LateUpdaterComponent : public virtual BaseLateUpdaterComponent
		{
			friend class GameObject;
		protected:
			LateUpdaterComponent(GameObject* gameObject)
				: BaseLateUpdaterComponent(gameObject)
			{

			}

			/// <summary>
			/// If this function is override, it must call the LateUpdaterComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				ObjectManager::GetInstance().AddToLateUpdate(this);
			}

			/// <summary>
			/// If this function is override, it must call the LateUpdaterComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				ObjectManager::GetInstance().RemoveFromLateUpdate(this);
			}
		};
	}
}