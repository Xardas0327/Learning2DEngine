#pragma once

#include "Component.h"

namespace Learning2DEngine
{
	namespace System
	{
		/// <summary>
		/// The classes, which are inherited from UpdaterComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about `Component`.
		/// </summary>
		class LateUpdaterComponent : public virtual Component
		{
			friend class GameObject;
		protected:
			LateUpdaterComponent(GameObject* gameObject);

			/// <summary>
			/// If this function is override, it must call the LateUpdaterComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override;

			/// <summary>
			/// If this function is override, it must call the LateUpdaterComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override;

		public:
			virtual void LateUpdate() = 0;
		};
	}
}