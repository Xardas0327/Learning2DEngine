#include "DebugBoxColliderRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "DebugRenderData.h"
#include "DebugMacro.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../System/ComponentManager.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"
#include "../Physics/BaseBoxColliderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		DebugBoxColliderRenderer::DebugBoxColliderRenderer()
			: shader(), vao(0), vbo(0), renderData()
		{

		}

		void DebugBoxColliderRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SIMPLE_COLOR_NAME)
				? resourceManager.GetShader(ShaderConstant::SIMPLE_COLOR_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::SIMPLE_COLOR_NAME,
					ShaderConstant::GetSimpleColorVertexShader(),
					ShaderConstant::GetSimpleColorFragmentShader());
		}

		void DebugBoxColliderRenderer::InitVao()
		{
			//positions
			float vertices[] = {
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,
				0.0f, 1.0f,
			};
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void DebugBoxColliderRenderer::Init()
		{
			if (ComponentManager::GetInstance().GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(RenderManager::GetInstance().mutex);
				InitShader();
				InitVao();
			}
			else
			{
				InitShader();
				InitVao();
			}
		}

		void DebugBoxColliderRenderer::DestroyObject()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);

			renderData.clear();
		}

		void DebugBoxColliderRenderer::Destroy()
		{
			if (ComponentManager::GetInstance().GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(RenderManager::GetInstance().mutex);
				DestroyObject();
			}
			else
			{
				DestroyObject();
			}
		}

		void DebugBoxColliderRenderer::SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData)
		{
			this->renderData = renderData;
		}

		void DebugBoxColliderRenderer::Draw(int layer)
		{
			if (renderData.find(layer) == renderData.end())
				return;

			shader.Use();
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());

			glBindVertexArray(vao);

			for (auto data : renderData[layer])
			{
				auto colliderData = static_cast<DebugRenderData<BaseBoxColliderComponent>*>(data);
				if (!colliderData->objectComponent->isActive)
					continue;

				glm::mat4 model = glm::mat4(1.0f);

				model = glm::translate(model, 
					glm::vec3(colliderData->objectComponent->gameObject->transform.GetPosition() + colliderData->objectComponent->colliderOffset, 0.0f)
				);
				model = glm::scale(model, glm::vec3(colliderData->objectComponent->colliderSize, 1.0f));

				shader.SetMatrix4("model", model);
				shader.SetVector4f("color", 
					colliderData->objectComponent->mode == ColliderMode::TRIGGER
					? L2DE_DEBUG_SHOW_COLLIDER_TRIGGER_COLOR
					: L2DE_DEBUG_SHOW_COLLIDER_COLOR
				);

				glDrawArrays(GL_LINE_LOOP, 0, 4);
			}

			glBindVertexArray(0);
		}
	}
}