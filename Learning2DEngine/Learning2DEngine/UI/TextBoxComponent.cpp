#include "TextBoxComponent.h"

#include "../System/GameObjectManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

    namespace UI
    {
		TextBoxComponent::TextBoxComponent(
			GameObject* gameObject,
			SimpleText2DRenderComponent& textComponent,
			TextBoxMode textBoxMode,
			int layer,
			glm::vec4 color)
			: LateUpdaterComponent(gameObject), Component(gameObject),
			  textRenderData(&textComponent.data), simpleRenderComponent(nullptr), multiRenderComponent(nullptr),
			initLayer(layer), initColor(color), initRendererMode(textComponent.mode), textBoxMode(textBoxMode)
		{

		}

		TextBoxComponent::TextBoxComponent(
			GameObject* gameObject,
			Text2DRenderComponent& textComponent,
			TextBoxMode textBoxMode,
			int layer,
			glm::vec4 color)
			: LateUpdaterComponent(gameObject), Component(gameObject),
			textRenderData(&textComponent.data), simpleRenderComponent(nullptr), multiRenderComponent(nullptr),
			initLayer(layer), initColor(color), initRendererMode(textComponent.mode), textBoxMode(textBoxMode)
		{

		}

		void TextBoxComponent::Init()
		{
			LateUpdaterComponent::Init();

			auto renderGameObject = GameObjectManager::GetInstance().CreateGameObject();
			switch (textBoxMode)
			{
				case TextBoxMode::SIMPLE:
					simpleRenderComponent = renderGameObject->AddComponent<SimpleSpriteRenderComponent>(
						initRendererMode,
						initLayer,
						initColor
					);
					simpleRenderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
					break;
				case TextBoxMode::MULTI:
					multiRenderComponent = renderGameObject->AddComponent<SpriteRenderComponent>(
						initRendererMode,
						initLayer,
						initColor
					);
					multiRenderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
					break;
			}
		}

		void TextBoxComponent::Destroy()
		{
			LateUpdaterComponent::Destroy();

			switch (textBoxMode)
			{
				case TextBoxMode::SIMPLE:
					GameObjectManager::GetInstance().DestroyGameObject(simpleRenderComponent);
					break;
				case TextBoxMode::MULTI:
					GameObjectManager::GetInstance().DestroyGameObject(multiRenderComponent);
					break;
			}
		}

		void TextBoxComponent::LateUpdate()
		{
			auto textLength = textRenderData->GetTextSize();
			switch (textBoxMode)
			{
				case TextBoxMode::SIMPLE:
					simpleRenderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
					simpleRenderComponent->gameObject->transform = Transform(
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
					break;
				case TextBoxMode::MULTI:
					multiRenderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
					multiRenderComponent->gameObject->transform = Transform(
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
					break;
			}
		}
    }
}