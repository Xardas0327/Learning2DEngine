#include "ParticleSystem.h"

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
        int ParticleSystem::referenceNumber = 0;
        Shader ParticleSystem::shader;

        unsigned int ParticleSystem::vao = 0;
        unsigned int ParticleSystem::vbo = 0;
        unsigned int ParticleSystem::ebo = 0;

		ParticleSystem::ParticleSystem(
			GameObject* gameObject,
			unsigned int particleAmount,
			ParticleSettings* particleSettings,
			int renderLayer)
			: Component(gameObject), BaseRendererComponent(gameObject, renderLayer), RendererComponent(gameObject, renderLayer),
			BaseUpdaterComponent(gameObject), UpdaterComponent(gameObject),
			isRunning(false), particleAmount(particleAmount), systemSettings(),
			texture(nullptr), delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0),
			particles(nullptr),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{
		}

		ParticleSystem::ParticleSystem(
			GameObject* gameObject,
			unsigned int particleAmount,
			const Texture2D& texture,
			const ParticleSystemSettings& systemSettings,
			ParticleSettings* particleSettings,
			int renderLayer)
			: Component(gameObject), BaseRendererComponent(gameObject, renderLayer), RendererComponent(gameObject, renderLayer),
			BaseUpdaterComponent(gameObject), UpdaterComponent(gameObject),
			isRunning(false), particleAmount(particleAmount), systemSettings(systemSettings),
			delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0),
			particles(nullptr), texture(new Texture2D(texture)),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{

		}

		ParticleSystem::~ParticleSystem()
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

		void ParticleSystem::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();


			ParticleSystem::shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::SPRITE_SHADER_NAME,
					ShaderConstant::SPRITE_VERTEX_SHADER,
					ShaderConstant::SPRITE_FRAGMENT_SHADER);
			ParticleSystem::shader.SetInteger("spriteTexture", 0);
		}

		void ParticleSystem::InitVao()
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

			glGenVertexArrays(1, &ParticleSystem::vao);
			glGenBuffers(1, &ParticleSystem::vbo);
			glGenBuffers(1, &ParticleSystem::ebo);
			glBindVertexArray(ParticleSystem::vao);
			glBindBuffer(GL_ARRAY_BUFFER, ParticleSystem::vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticleSystem::ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void ParticleSystem::Init()
		{
			RendererComponent::Init();
			UpdaterComponent::Init();
			particles = new Particle[particleAmount];

			// If nothing use it
			if (!ParticleSystem::referenceNumber)
			{
				InitShader();
				InitVao();
			}
			++ParticleSystem::referenceNumber;
		}

		void ParticleSystem::Destroy()
		{
			RendererComponent::Destroy();
			UpdaterComponent::Destroy();
			delete[] particles;
			particles = nullptr;

			if (IsRunning())
			{
				Stop();
			}

			--ParticleSystem::referenceNumber;
			// If nothing use it
			if (!ParticleSystem::referenceNumber)
			{
				glDeleteVertexArrays(1, &ParticleSystem::vao);
				glDeleteBuffers(1, &ParticleSystem::vbo);
				glDeleteBuffers(1, &ParticleSystem::ebo);
			}
		}

		void ParticleSystem::Draw()
		{
			if (!isRunning || delayTime > 0.0f)
				return;

			auto& renderManager = RenderManager::GetInstance();
			BlendFuncFactor previousBlendFuncFactor = renderManager.GetBlendFunc();

			if (systemSettings.isUseBlend)
			{
				renderManager.SetBlendFunc(systemSettings.blendFuncFactor);
			}

			ParticleSystem::shader.Use();
			if (IsUseTexture())
			{
				glActiveTexture(GL_TEXTURE0);
				texture->Bind();
			}

			glBindVertexArray(ParticleSystem::vao);

			for (int i = 0; i < particleAmount; ++i)
			{
				if (particles[i].lifeTime > 0.0f)
				{
					glm::mat4 model = glm::mat4(1.0f);
					// first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
					model = glm::translate(model, glm::vec3(particles[i].transform.position, 0.0f));
					// move origin of rotation to center of quad
					model = glm::translate(model, glm::vec3(0.5f * particles[i].transform.scale.x, 0.5f * particles[i].transform.scale.y, 0.0f));
					// then rotate
					model = glm::rotate(model, glm::radians(particles[i].transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
					// move origin back
					model = glm::translate(model, glm::vec3(-0.5f * particles[i].transform.scale.x, -0.5f * particles[i].transform.scale.y, 0.0f));
					// then rotate
					model = glm::scale(model, glm::vec3(particles[i].transform.scale, 1.0f)); // last scale

					ParticleSystem::shader.SetMatrix4("model", model);
					ParticleSystem::shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
					ParticleSystem::shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());
					ParticleSystem::shader.SetVector3f("spriteColor", particles[i].color);
					ParticleSystem::shader.SetInteger("isUseTexture", IsUseTexture());

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

		void ParticleSystem::Start()
		{
			particleSettings->Init(*gameObject);

			delayTime = systemSettings.delayBeforeStart;
			nextSpawnTime = 0.0f;
			lastUsedParticleIndex = 0;
			isRunning = true;
		}

		void ParticleSystem::Update()
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

		void ParticleSystem::Stop()
		{
			particleSettings->Destroy();
			isRunning = false;
		}

		void ParticleSystem::UpdateActiveParticles()
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

		void ParticleSystem::TryToSpawnNewParticles()
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

		void ParticleSystem::SpawnNewParticles()
		{
			for (int i = 0; i < systemSettings.newParticlesPerSpawn; ++i)
			{
				unsigned int index = GetUnusedParticleIndex();
				particleSettings->SpawnParticle(particles[index], *gameObject);
			}
		}

		unsigned int ParticleSystem::GetUnusedParticleIndex()
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

		void ParticleSystem::ClearTexture()
		{
			if (IsUseTexture())
			{
				delete texture;
				texture = nullptr;
			}
		}
	}
}