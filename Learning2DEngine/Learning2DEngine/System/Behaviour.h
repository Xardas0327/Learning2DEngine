#pragma once

#include "Component.h"

namespace Learning2DEngine
{
	namespace System
	{
		/// <summary>
		/// The classes, which are inherited from Behaviour,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, It is recommand, that the constructor of the inherited class is protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Behaviour : public virtual Component
		{
			friend class GameObject;
		protected:
			Behaviour(System::GameObject* gameObject)
				: Component(gameObject)
			{

			}
		};
	}
}