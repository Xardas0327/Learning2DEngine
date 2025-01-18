#pragma once

#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from Renderer,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check more info about System::Component
		/// </summary>
		// TODO: This class have to be removed
		class Renderer : public virtual System::Component
		{
			friend class System::GameObject;
		protected:
			Renderer(System::GameObject* gameObject)
				: Component(gameObject)
			{

			}
		public:
			virtual void Draw() = 0;
		};
	}
}