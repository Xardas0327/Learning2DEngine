#pragma once

#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from Renderer,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, It is recommand, that the constructor of the inherited class is protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Renderer : public virtual System::Component
		{
			friend class System::GameObject;
		protected:
			Renderer(System::GameObject* gameObject)
				: Component(gameObject)
			{

			}
		public:
			virtual void Init() = 0;
			virtual void Destroy() = 0;

			virtual void Draw() = 0;
		};
	}
}