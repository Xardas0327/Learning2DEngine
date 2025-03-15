#include "OldParticleSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include "BasicParticleSettings.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"
#include "../System/ResourceManager.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace ParticleSimulator
	{
        int OldParticleSystem::referenceNumber = 0;
        Shader OldParticleSystem::shader;

        unsigned int OldParticleSystem::vao = 0;
        unsigned int OldParticleSystem::vbo = 0;
        unsigned int OldParticleSystem::ebo = 0;

		OldParticleSystem::OldParticleSystem(
			GameObject* gameObject,
			unsigned int particleAmount,
			ParticleSettings* particleSettings,
			int renderLayer)
			: Component(gameObject), OldBaseRendererComponent(gameObject, renderLayer), OldRendererComponent(gameObject, renderLayer),
			BaseUpdaterComponent(gameObject), UpdaterComponent(gameObject),
			isRunning(false), particleAmount(particleAmount), systemSettings(),
			texture(nullptr), delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0),
			particles(nullptr),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{
		}

		OldParticleSystem::OldParticleSystem(
			GameObject* gameObject,
			unsigned int particleAmount,
			const Texture2D& texture,
			const ParticleSystemSettings& systemSettings,
			ParticleSettings* particleSettings,
			int renderLayer)
			: Component(gameObject), OldBaseRendererComponent(gameObject, renderLayer), OldRendererComponent(gameObject, renderLayer),
			BaseUpdaterComponent(gameObject), UpdaterComponent(gameObject),
			isRunning(false), particleAmount(particleAmount), systemSettings(systemSettings),
			delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0),
			particles(nullptr), texture(new Texture2D(texture)),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{

		}

		OldParticleSystem::~OldParticleSystem()
		{
			if (IsRunning())
			{
				Stop();
			}

			if (IsUseTexture())
			{
				delete texture;
			}
			if (particleSettings != nullptr)
			{
				delete particleSettings;
			}
		}

		void OldParticleSystem::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			OldParticleSystem::shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::SPRITE_SHADER_NAME,
					ShaderConstant::SPRITE_VERTEX_SHADER,
					ShaderConstant::SPRITE_FRAGMENT_SHADER);
		}

		void OldParticleSystem::InitVao()
		{
			float vertices[] = {
				// pos      // tex
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
			};

			unsigned int indices[] = {
			   0, 1, 3,
			   1, 2, 3
			};

			glGenVertexArrays(1, &OldParticleSystem::vao);
			glGenBuffers(1, &OldParticleSystem::vbo);
			glGenBuffers(1, &OldParticleSystem::ebo);
			glBindVertexArray(OldParticleSystem::vao);
			glBindBuffer(GL_ARRAY_BUFFER, OldParticleSystem::vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OldParticleSystem::ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void OldParticleSystem::Init()
		{
			OldRendererComponent::Init();
			UpdaterComponent::Init();
			particles = new Particle[particleAmount];

			// If nothing use it
			if (!OldParticleSystem::referenceNumber)
			{
				InitShader();
				InitVao();
			}
			++OldParticleSystem::referenceNumber;
		}

		void OldParticleSystem::Destroy()
		{
			OldRendererComponent::Destroy();
			UpdaterComponent::Destroy();
			delete[] particles;
			particles = nullptr;

			if (IsRunning())
			{
				Stop();
			}

			--OldParticleSystem::referenceNumber;
			// If nothing use it
			if (!OldParticleSystem::referenceNumber)
			{
				glDeleteVertexArrays(1, &OldParticleSystem::vao);
				glDeleteBuffers(1, &OldParticleSystem::vbo);
				glDeleteBuffers(1, &OldParticleSystem::ebo);
			}
		}

		void OldParticleSystem::Draw()
		{
			if (!isRunning || delayTime > 0.0f)
				return;

			auto& renderManager = RenderManager::GetInstance();
			BlendFuncFactor previousBlendFuncFactor = renderManager.GetBlendFunc();

			if (systemSettings.isUseBlend)
			{
				renderManager.SetBlendFunc(systemSettings.blendFuncFactor);
			}

			OldParticleSystem::shader.Use();
			OldParticleSystem::shader.SetInteger("spriteTexture", 0);
			if (IsUseTexture())
			{
				glActiveTexture(GL_TEXTURE0);
				texture->Bind();
			}

			glBindVertexArray(OldParticleSystem::vao);

			for (int i = 0; i < particleAmount; ++i)
			{
				if (particles[i].lifeTime > 0.0f)
				{
					OldParticleSystem::shader.SetMatrix4("model", particles[i].transform.GetModelMatrix());
					OldParticleSystem::shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
					OldParticleSystem::shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());
					OldParticleSystem::shader.SetVector3f("spriteColor", particles[i].color);
					OldParticleSystem::shader.SetInteger("isUseTexture", IsUseTexture());

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);


			if (systemSettings.isUseBlend)
			{
				renderManager.SetBlendFunc(previousBlendFuncFactor);
			}
		}

		void OldParticleSystem::Start()
		{
			particleSettings->Init(*gameObject);

			delayTime = systemSettings.delayBeforeStart;
			nextSpawnTime = 0.0f;
			lastUsedParticleIndex = 0;
			isRunning = true;
		}

		void OldParticleSystem::Update()
		{
			if (!isRunning)
				return;

			if (delayTime > 0.0f)
			{
				delayTime -= Game::GetDeltaTime();
				return;
			}

			UpdateActiveParticles();
			TryToSpawnNewParticles();
		}

		void OldParticleSystem::Stop()
		{
			particleSettings->Destroy();
			isRunning = false;
		}

		void OldParticleSystem::UpdateActiveParticles()
		{
			for (int i = 0; i < particleAmount; ++i)
			{
				particles[i].lifeTime -= Game::GetDeltaTime();
				if (particles[i].lifeTime > 0.0f)
				{
					particleSettings->UpdateParticle(particles[i], *gameObject);
				}
			}
		}

		void OldParticleSystem::TryToSpawnNewParticles()
		{
			nextSpawnTime -= Game::GetDeltaTime();

			float spawnTime = systemSettings.spawnTime < 0.0f ? 0.0f : systemSettings.spawnTime;

			if (spawnTime == 0.0f)
			{
				nextSpawnTime = 0.0f;
				SpawnNewParticles();
			}
			else
			{
				while (nextSpawnTime <= 0.0f)
				{
					nextSpawnTime += spawnTime;

					SpawnNewParticles();
				}
			}
		}

		void OldParticleSystem::SpawnNewParticles()
		{
			for (int i = 0; i < systemSettings.newParticlesPerSpawn; ++i)
			{
				unsigned int index = GetUnusedParticleIndex();
				particleSettings->SpawnParticle(particles[index], *gameObject);
			}
		}

		unsigned int OldParticleSystem::GetUnusedParticleIndex()
		{
			for (unsigned int i = lastUsedParticleIndex; i < particleAmount; ++i) {
				if (particles[i].lifeTime <= 0.0f) {
					lastUsedParticleIndex = i;
					return i;
				}
			}

			for (unsigned int i = 0; i < lastUsedParticleIndex; ++i) {
				if (particles[i].lifeTime <= 0.0f) {
					lastUsedParticleIndex = i;
					return i;
				}
			}

			//It has to have something. So this is the default.
			lastUsedParticleIndex = 0;
			return 0;
		}

		void OldParticleSystem::ClearTexture()
		{
			if (IsUseTexture())
			{
				delete texture;
				texture = nullptr;
			}
		}
	}
}