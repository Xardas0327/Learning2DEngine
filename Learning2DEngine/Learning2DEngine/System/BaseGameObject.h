#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		// It exists only for GameObjectManager.
		// Do not use it.
		class BaseGameObject
		{
		protected:
			BaseGameObject() {};
		public:
			virtual ~BaseGameObject() {};
		};
	}
}