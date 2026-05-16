#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject;

		/// <summary>
		/// The classes, which are inherited from Component,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, it is recommended, that the constructor, the Init() and Destroy() of the inherited class is protected
		/// and only the GameObject can use them.
		/// </summary>
		class Component
		{
			friend class GameObject;
		protected:
			bool isActive;

			Component(GameObject* gameObject);

			virtual void Init();
			virtual void Destroy();
		public:
			GameObject* const gameObject;

			virtual ~Component() = default;

			//It returns, that the component is active or not.
			inline bool GetActive() const
			{
				return isActive;
			}

			void SetActive(bool active);

			//It returns, that the component and its gameobject is active or not.
			bool IsActive() const;
		};
	}
}