#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject;

		/// <summary>
		/// The classes, which are inherited from Component,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, It is recommand, that the constructor of the inherited class is protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Component
		{
			friend class GameObject;
		protected:
			bool isActive;

			Component(GameObject* gameObject)
				: gameObject(gameObject), isActive(true)
			{

			}
		public:
			GameObject* const gameObject;

			virtual ~Component() {}

			virtual void Init() {}

			virtual void Destroy() {}

			inline void SetActive(bool value)
			{
				isActive = value;
			}

			inline bool GetActive()
			{
				return isActive;
			}
		};
	}
}