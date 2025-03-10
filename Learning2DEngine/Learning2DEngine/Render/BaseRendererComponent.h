#pragma once

#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from BaseRendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about System::Component
		/// </summary>
		class BaseRendererComponent : public virtual System::Component
		{
		private:
			int layer;
		protected:
			BaseRendererComponent(System::GameObject* gameObject)
				: System::Component(gameObject), layer(0)
			{

			}

			BaseRendererComponent(System::GameObject* gameObject, int layer)
				: System::Component(gameObject), layer(layer)
			{

			}
		public:
			virtual void Draw() = 0;

			virtual void SetLayer(int value)
			{
				layer = value;
			}

			inline int GetLayer() const
			{
				return layer;
			}
		};
	}
}