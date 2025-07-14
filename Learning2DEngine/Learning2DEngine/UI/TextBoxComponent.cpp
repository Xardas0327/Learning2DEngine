#include "TextBoxComponent.h"

#include "../System/GameObjectManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

    namespace UI
    {
		TextBoxComponent::TextBoxComponent(GameObject* gameObject, SimpleText2DRenderComponent& textComponent, int layer, glm::vec4 color)
			: LateUpdaterComponent(gameObject), Component(gameObject),
			  textRenderData(&textComponent.data), renderComponent(nullptr),
			initLayer(layer), initColor(color), initRendererMode(textComponent.mode)
		{

		}

		TextBoxComponent::TextBoxComponent(GameObject* gameObject, Text2DRenderComponent& textComponent, int layer, glm::vec4 color)
			: LateUpdaterComponent(gameObject), Component(gameObject),
			textRenderData(&textComponent.data), renderComponent(nullptr),
			initLayer(layer), initColor(color), initRendererMode(textComponent.mode)
		{

		}

		void TextBoxComponent::Init()
		{
			LateUpdaterComponent::Init();

			auto renderGameObject = GameObjectManager::GetInstance().CreateGameObject();
			renderComponent = renderGameObject->AddComponent<SimpleSpriteRenderComponent>(
				initRendererMode,
				initLayer,
				initColor
			);
			renderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
		}

		void TextBoxComponent::Destroy()
		{
			LateUpdaterComponent::Destroy();

			GameObjectManager::GetInstance().DestroyGameObject(renderComponent);
		}

		void TextBoxComponent::LateUpdate()
		{
			auto textLength = textRenderData->GetTextLength();
			renderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
			renderComponent->gameObject->transform = Transform(
				glm::vec2(
					textRenderData->component->gameObject->transform.GetPosition().x - paddingLeftRight,
					textRenderData->component->gameObject->transform.GetPosition().y - paddingTopBottom
				),
				glm::vec2(
					textLength.x + 2.0f * paddingLeftRight,
					textLength.y + 2.0f * paddingTopBottom
				),
				textRenderData->component->gameObject->transform.GetRotation()
			);
		}
    }
}