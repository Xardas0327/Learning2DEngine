#pragma once

#include "../System/GameObject.h"
#include "../System/LateUpdaterComponent.h"
#include "../UI/SimpleText2DRenderComponent.h"
#include "../UI/TextBoxComponent.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		// It is used in Render Mode only.
		// The DebugPosition will be updated in a LateUpdate().
		// So, it can work well only if the position of the GameObject is updated in Update() function(s).
		// Moreover, it works well with renderers if the renderers use the camera view matrix.
		class DebugPosition : public System::LateUpdaterComponent
		{
			friend class System::GameObject;
		private:
			UI::SimpleText2DRenderComponent* textComponent;
			UI::TextBoxComponent* boxComponent;
			static UI::FontSizePair fontSizePair;
			static bool isInited;

			DebugPosition(System::GameObject* gameObject);

			void Init() override;
			void Destroy() override;
			void LateUpdate() override;
		public:

			static void Init(const UI::FontSizePair& fontSizePair);

			void SetActive(bool value);

			inline void SetTextLayer(int value)
			{
				textComponent->SetLayer(value);
			}

			inline int GetTextLayer() const
			{
				return textComponent->GetLayer();
			}

			inline void SetBoxLayer(int value)
			{
				boxComponent->SetLayer(value);
			}

			inline int GetBoxLayer() const
			{
				return boxComponent->GetLayer();
			}

			inline void SetTextColor(glm::vec4 color)
			{
				textComponent->data.color = color;
			}

			inline glm::vec4 GetTextColor() const
			{
				return textComponent->data.color;
			}

			inline void SetBoxColor(glm::vec4 color)
			{
				boxComponent->SetColor(color);
			}

			inline glm::vec4 GetBoxColor() const
			{
				return boxComponent->GetColor();
			}
		};
	}
}