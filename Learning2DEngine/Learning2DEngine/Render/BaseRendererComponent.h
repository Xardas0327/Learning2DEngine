#pragma once

#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from BaseRendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check more info about System::Component
		/// </summary>
		class BaseRendererComponent : public virtual System::Component
		{
		protected:
			BaseRendererComponent(System::GameObject* gameObject)
				: System::Component(gameObject)
			{

			}
		public:
			virtual void Draw() = 0;
		};
	}
}