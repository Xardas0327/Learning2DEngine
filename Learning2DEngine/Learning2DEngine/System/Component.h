#pragma once

namespace Learning2DEngine
{
	namespace Render
	{
		class BaseRendererComponent;
	}
	namespace System
	{
		class GameObject;
		class BaseUpdaterComponent;

		/// <summary>
		/// The classes, which are inherited from Component,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, it is recommended, the Init() and Destroy() of the inherited class is protected
		/// and only the GameObject can use them.
		/// </summary>
		class Component
		{
			friend class GameObject;
			friend class BaseUpdaterComponent;
			friend class Render::BaseRendererComponent;
		private:
			bool isActive;

			Component(GameObject* gameObject)
				: gameObject(gameObject), isActive(true)
			{

			}

		protected:
			virtual void Init() = 0;
			virtual void Destroy() = 0;

		public:
			GameObject* const gameObject;

			virtual ~Component() {}

			inline void SetActive(bool value)
			{
				isActive = value;
			}

			inline bool GetActive() const
			{
				return isActive;
			}
		};
	}
}