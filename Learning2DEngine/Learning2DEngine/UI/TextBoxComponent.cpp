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
					simpleRenderComponent->gameObject->SetActive(textRenderData->component->IsActive());
					if (!simpleRenderComponent->gameObject->IsActive())
						return;

					simpleRenderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
					simpleRenderComponent->gameObject->transform.SetLocalPosition(
						glm::vec2(
							textRenderData->component->gameObject->transform.GetGlobalPosition().x - paddingLeftRight,
							textRenderData->component->gameObject->transform.GetGlobalPosition().y - paddingTopBottom
						)
					);
					simpleRenderComponent->gameObject->transform.SetLocalScale(
						glm::vec2(
							textLength.x + 2.0f * paddingLeftRight,
							textLength.y + 2.0f * paddingTopBottom
						)
					);
					simpleRenderComponent->gameObject->transform.SetLocalRotation(
						textRenderData->component->gameObject->transform.GetGlobalRotation()
					);
					break;
				case TextBoxMode::MULTI:
					multiRenderComponent->gameObject->SetActive(textRenderData->component->IsActive());
					if (!multiRenderComponent->gameObject->IsActive())
						return;

					multiRenderComponent->data.isUseCameraView = textRenderData->isUseCameraView;
					multiRenderComponent->gameObject->transform.SetLocalPosition(
						glm::vec2(
							textRenderData->component->gameObject->transform.GetGlobalPosition().x - paddingLeftRight,
							textRenderData->component->gameObject->transform.GetGlobalPosition().y - paddingTopBottom
						)
					);
					multiRenderComponent->gameObject->transform.SetLocalScale(
						glm::vec2(
							textLength.x + 2.0f * paddingLeftRight,
							textLength.y + 2.0f * paddingTopBottom
						)
					);
					multiRenderComponent->gameObject->transform.SetLocalRotation(
						textRenderData->component->gameObject->transform.GetGlobalRotation()
					);
					break;
			}
		}

		void TextBoxComponent::SetPadding(float padding)
		{
			paddingTopBottom = padding;
			paddingLeftRight = padding;
		}

		int TextBoxComponent::GetLayer() const
		{
			switch (textBoxMode)
			{
			case TextBoxMode::SIMPLE:
				return simpleRenderComponent->GetLayer();
				break;
			case TextBoxMode::MULTI:
				return multiRenderComponent->GetLayer();
				break;
			}

			return 0;
		}

		void TextBoxComponent::SetLayer(int layer)
		{
			switch (textBoxMode)
			{
			case TextBoxMode::SIMPLE:
				simpleRenderComponent->SetLayer(layer);
				break;
			case TextBoxMode::MULTI:
				multiRenderComponent->SetLayer(layer);
				break;
			}
		}

		glm::vec4 TextBoxComponent::GetColor() const
		{
			switch (textBoxMode)
			{
			case TextBoxMode::SIMPLE:
				return simpleRenderComponent->data.color;
				break;
			case TextBoxMode::MULTI:
				return multiRenderComponent->data.color;
				break;
			}

			return glm::vec4(1.0f);
		}

		void TextBoxComponent::SetColor(const glm::vec4& color)
		{
			switch (textBoxMode)
			{
			case TextBoxMode::SIMPLE:
				simpleRenderComponent->data.color = color;
				break;
			case TextBoxMode::MULTI:
				multiRenderComponent->data.color = color;
				break;
			}
		}
    }
}