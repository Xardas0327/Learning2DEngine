#include "DebugPosition.h"

#include <sstream>
#include <iomanip>

#include "DebugMacro.h"
#include "../System/GameObjectManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace UI;

	namespace DebugTool
	{
		UI::FontSizePair DebugPosition::fontSizePair;
		bool DebugPosition::isInited = false;

		DebugPosition::DebugPosition(GameObject* gameObject)
			:LateUpdaterComponent(gameObject), Component(gameObject),
			isFirstUpdate(true), lastPosition(),
			textComponent(nullptr), boxComponent(nullptr)
		{

		}

		void DebugPosition::Init(const UI::FontSizePair& fontSizePair)
		{
			DebugPosition::fontSizePair = fontSizePair;
			DebugPosition::isInited = true;
		}

		void DebugPosition::Init()
		{
			LateUpdaterComponent::Init();

			if (!DebugPosition::isInited)
				return;

			auto& gameObjectManager = GameObjectManager::GetInstance();

			auto textGo = GameObjectManager::GetInstance().CreateGameObject();
			textComponent = textGo->AddComponent<Text2DRenderComponent>(
				RendererMode::RENDER,
				DebugPosition::fontSizePair,
				L2DE_DEBUG_SHOW_POSITION_DEFAULT_TEXT_LAYER,
				L2DE_DEBUG_SHOW_POSITION_TEXT_COLOR
			);
			textComponent->data.isUseCameraView = true;

			auto boxGo = GameObjectManager::GetInstance().CreateGameObject();
			boxComponent = boxGo->AddComponent<TextBoxComponent>(
				*textComponent,
				TextBoxMode::MULTI,
				L2DE_DEBUG_SHOW_POSITION_DEFAULT_BOX_LAYER,
				L2DE_DEBUG_SHOW_POSITION_BOX_COLOR
			);
			boxComponent->SetPadding(L2DE_DEBUG_SHOW_POSITION_BOX_PADDING);

			SetActive(L2DE_DEBUG_SHOW_POSITION_DEFAULT_VALUE);
		}

		void DebugPosition::Destroy()
		{
			LateUpdaterComponent::Destroy();

			auto& gameObjectManager = GameObjectManager::GetInstance();

			if (textComponent != nullptr)
				gameObjectManager.DestroyGameObject(textComponent);

			if (boxComponent != nullptr)
				gameObjectManager.DestroyGameObject(boxComponent);

		}

		void DebugPosition::LateUpdate()
		{
			if(isFirstUpdate || 
				gameObject->transform.GetPosition() != lastPosition)
			{
				isFirstUpdate = false;
				lastPosition = gameObject->transform.GetPosition();

				std::ostringstream oss;
				oss << std::fixed << std::setprecision(2);
				oss << "X: " << lastPosition.x << " Y: " << lastPosition.y;

				textComponent->data.SetText(oss.str());
				textComponent->gameObject->transform.SetPosition(lastPosition);
			}
		}

		void DebugPosition::SetActive(bool value)
		{
			isActive = value;
			textComponent->gameObject->isActive = value;
			boxComponent->gameObject->isActive = value;
		}
	}
}