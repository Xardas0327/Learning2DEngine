#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "../Render/LateRendererComponent.h"
#include "../Render/RendererMode.h"

#include "Text2DRenderData.h"
#include "SimpleText2DRenderer.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class SimpleText2DLateRenderComponent : public Render::LateRendererComponent<Text2DRenderData, SimpleText2DRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;
			/// <summary>
			/// It is counted, that how many SimpleText2DLateRenderComponent exist.
			/// </summary>
			static int refrenceNumber;
			static std::mutex mutex;

		protected:
			SimpleText2DLateRenderComponent(
				System::GameObject* gameObject,
				const FontSizePair& fontSizePair,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));
			SimpleText2DLateRenderComponent(
				System::GameObject* gameObject,
				const FontSizePair& fontSizePair,
				const std::string& text,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;

			const std::string& GetId() const override;

			SimpleText2DRenderer* GetRenderer() const override;
		};
	}
}