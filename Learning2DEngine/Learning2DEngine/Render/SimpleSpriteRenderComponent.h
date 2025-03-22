#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "RendererComponent.h"
#include "SimpleSpriteRenderer.h"
#include "SpriteRenderData.h"
#include "Texture2D.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class SimpleSpriteRenderComponent : public virtual RendererComponent<SpriteRenderData, SimpleSpriteRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;
			/// <summary>
			/// It is counted, that how many SimpleSpriteRenderComponent exist.
			/// </summary>
			static int refrenceNumber;
		protected:
			std::mutex mutex;

			SimpleSpriteRenderComponent(System::GameObject* gameObject, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
			SimpleSpriteRenderComponent(System::GameObject* gameObject, const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;

			const std::string& GetId() const override;

			SimpleSpriteRenderer* GetRenderer() const override;
		};
	}
}