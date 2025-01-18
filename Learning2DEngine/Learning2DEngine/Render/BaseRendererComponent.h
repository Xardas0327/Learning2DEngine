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

			void SetLayer(int value)
			{
				layer = value;
				// TODO: trigger the reorder
			}

			inline int GetLayer() const
			{
				return layer;
			}
		};
	}
}