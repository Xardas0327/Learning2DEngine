#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "OldRendererComponent.h"
#include "MultiSpriteRenderer.h"
#include "SpriteRenderData.h"
#include "Texture2D.h"

namespace Learning2DEngine
{
	namespace Render
	{
		//It supports the multi instance rendering
		class SpriteRenderComponent : public OldRendererComponent<SpriteRenderData, MultiSpriteRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;
			/// <summary>
			/// It is counted, that how many SpriteRenderComponent exist.
			/// </summary>
			static int refrenceNumber;
			static std::mutex mutex;

		protected:
			SpriteRenderComponent(System::GameObject* gameObject, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
			SpriteRenderComponent(System::GameObject* gameObject, const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;

			const std::string& GetId() const override;

			MultiSpriteRenderer* GetRenderer() const override;
		};
	}
}