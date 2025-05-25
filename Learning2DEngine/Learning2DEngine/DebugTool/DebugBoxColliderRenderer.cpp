#include "DebugBoxColliderRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DebugMacro.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../System/ComponentManager.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		DebugBoxColliderRenderer::DebugBoxColliderRenderer()
			: shader(), vao(0), vboStatic(0), vboDynamic(0), maxObjectSize(0),
			debugRenderData(), dynamicData(nullptr)
		{

		}

		void DebugBoxColliderRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::BASE_COLOR_NAME)
				? resourceManager.GetShader(ShaderConstant::BASE_COLOR_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::BASE_COLOR_NAME,
					ShaderConstant::GetBaseColorVertexShader(),
					ShaderConstant::GetBaseColorFragmentShader());
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
			glGenBuffers(1, &vboStatic);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vboStatic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glGenBuffers(1, &vboDynamic);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(BaseColorDynamicData), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)offsetof(BaseColorDynamicData, modelMatrix));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)(offsetof(BaseColorDynamicData, modelMatrix) + sizeof(float) * 4));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)(offsetof(BaseColorDynamicData, modelMatrix) + sizeof(float) * 8));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)(offsetof(BaseColorDynamicData, modelMatrix) + sizeof(float) * 12));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)offsetof(BaseColorDynamicData, color));

			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			maxObjectSize = 1;
			dynamicData = new BaseColorDynamicData[maxObjectSize];
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
			glDeleteBuffers(1, &vboStatic);
			glDeleteBuffers(1, &vboDynamic);

			debugRenderData.clear();

			if (dynamicData != nullptr)
			{
				delete[] dynamicData;
			}
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
			debugRenderData.clear();
			size_t maxDynamicSize = 0;
			for (auto& layerData : renderData)
			{
				auto& actualLayerData = debugRenderData[layerData.first];

				for (auto& data : layerData.second)
				{
					auto colliderData = static_cast<DebugRenderData<BaseBoxColliderComponent>*>(data);
					if (!colliderData->objectComponent->isActive)
						continue;

					actualLayerData.push_back(colliderData);
				}

				if (maxDynamicSize < actualLayerData.size())
					maxDynamicSize = actualLayerData.size();
			}

			//if the size is not enough or too big, it will be reallocated.
			if (maxDynamicSize > maxObjectSize || maxObjectSize > maxDynamicSize * 2)
			{
				//It allocates 20% more space, so that it does not have to allocate again
				//if there are some dynamic renderers. 
				maxObjectSize = static_cast<size_t>(
					static_cast<float>(maxDynamicSize) * 1.2f
				);

				glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
				glBufferData(GL_ARRAY_BUFFER, sizeof(BaseColorDynamicData) * maxObjectSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				delete[] dynamicData;
				dynamicData = new BaseColorDynamicData[maxObjectSize];
			}
		}

		void DebugBoxColliderRenderer::Draw(int layer)
		{
			if (debugRenderData.find(layer) == debugRenderData.end())
				return;

			shader.Use();
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);

			for (int i = 0; i < debugRenderData[layer].size(); ++i)
			{
				auto colliderData = debugRenderData[layer][i];

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model,
					glm::vec3(colliderData->objectComponent->gameObject->transform.GetPosition() + colliderData->objectComponent->colliderOffset, 0.0f)
				);
				model = glm::scale(model, glm::vec3(colliderData->objectComponent->colliderSize, 1.0f));
				
				std::memcpy(dynamicData[i].modelMatrix,
					glm::value_ptr(model),
					sizeof(dynamicData[i].modelMatrix));

				if (colliderData->objectComponent->mode == ColliderMode::TRIGGER)
					std::memcpy(dynamicData[i].color,
						glm::value_ptr(L2DE_DEBUG_SHOW_COLLIDER_TRIGGER_COLOR),
						sizeof(dynamicData[i].color));
				else
					std::memcpy(dynamicData[i].color,
						glm::value_ptr(L2DE_DEBUG_SHOW_COLLIDER_COLOR),
						sizeof(dynamicData[i].color));
			}

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BaseColorDynamicData) * debugRenderData[layer].size(), dynamicData);
			glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, static_cast<GLsizei>(debugRenderData[layer].size()));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
}