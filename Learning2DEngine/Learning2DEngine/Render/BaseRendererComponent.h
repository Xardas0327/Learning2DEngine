#pragma once

#include <string>

#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from BaseRendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// The TRenderData should be a class, which is inhereted from IRenderData.
		/// The TRenderer should be a class, which is inhereted from IRenderer.
		/// Please check for more info about System::Component
		/// </summary>
		template<class TRenderData, class TRenderer>
		class BaseRendererComponent : public virtual System::Component
		{
		private:
			int layer;
		protected:
			BaseRendererComponent(System::GameObject* gameObject, int layer = 0)
				: System::Component(gameObject), layer(0), data(gameObject)
			{

			}
		public:
			TRenderData data;

			virtual const std::string& GetId() const
			{
				return typeid(TRenderData).name();
			}

			virtual TRenderer* GetRenderer() const = 0;

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