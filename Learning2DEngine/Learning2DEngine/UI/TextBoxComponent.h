#pragma once

#include "../System/GameObject.h"
#include "../System/LateUpdaterComponent.h"
#include "../Render/SimpleSpriteRenderComponent.h"
#include "../UI/SimpleText2DRenderComponent.h"
#include "../UI/Text2DRenderComponent.h"

namespace Learning2DEngine
{
    namespace UI
    {
		/// <summary>
		/// The TextBoxComponent will create a new GameObject with a SimpleSpriteRenderComponent.
		/// The SimpleSpriteRenderComponent will be updated in a LateUpdate(). So, if the text should be updated in a Update() function(s).
		/// </summary>
		class TextBoxComponent : public System::LateUpdaterComponent
		{
			friend class System::GameObject;
		protected:
			Text2DRenderData* const textRenderData;
			Render::SimpleSpriteRenderComponent* renderComponent;
			const int initLayer;
			const glm::vec4 initColor;
			const Render::RendererMode initRendererMode;

			TextBoxComponent(
				System::GameObject* gameObject,
				SimpleText2DRenderComponent& textComponent,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			TextBoxComponent(
				System::GameObject* gameObject,
				Text2DRenderComponent& textComponent,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;
			void LateUpdate() override;

		public:
			inline int GetLayer() const
			{
				return renderComponent->GetLayer();
			}

			inline void SetLayer(int layer)
			{
				renderComponent->SetLayer(layer);
			}

			inline glm::vec4 GetColor() const
			{
				return renderComponent->data.color;
			}

			inline void SetColor(const glm::vec4& color)
			{
				renderComponent->data.color = color;
			}
		};
    }
}