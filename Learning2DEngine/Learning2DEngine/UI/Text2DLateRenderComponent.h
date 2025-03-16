#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "../Render/LateRendererComponent.h"

#include "Text2DRenderData.h"
#include "Text2DRenderer.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
    namespace UI
    {
		class Text2DLateRenderComponent : public virtual Render::LateRendererComponent<Text2DRenderData, Text2DRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;
			/// <summary>
			/// It is counted, that how many Text2DLateRenderComponent exist.
			/// </summary>
			static int refrenceNumber;
		protected:
			std::mutex mutex;

			Text2DLateRenderComponent(
				System::GameObject* gameObject,
				const FontSizePair& fontSizePair,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));
			Text2DLateRenderComponent(
				System::GameObject* gameObject,
				const FontSizePair& fontSizePair,
				const std::string& text,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;

			const std::string& GetId() const override;

			Text2DRenderer* GetRenderer() const override;
		};
    }
}