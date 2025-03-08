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
				: System::Component(gameObject), layer(0), data()
			{

			}
			BaseRendererComponent(System::GameObject* gameObject, const TRenderData& data, int layer = 0)
				: System::Component(gameObject), layer(0), data(data)
			{

			}
		public:
			TRenderData data;

			virtual const std::string& GetId() const = 0;

			virtual TRenderer GetRenderer()
			{
				return TRenderer();
			};

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