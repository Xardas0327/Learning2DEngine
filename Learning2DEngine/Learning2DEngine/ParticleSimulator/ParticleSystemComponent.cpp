#include "ParticleSystemComponent.h"

#include "BasicParticleSettings.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace ParticleSimulator
	{
		const std::string ParticleSystemComponent::id = "L2DE_ParticleSystemComponent";
		int ParticleSystemComponent::refrenceNumber = 0;

		ParticleSystemComponent::ParticleSystemComponent(
			GameObject* gameObject,
			unsigned int particleAmount,
			ParticleSettings* particleSettings,
			int renderLayer)
			: RendererComponent(gameObject, renderLayer, particleAmount), BaseRendererComponent(gameObject, renderLayer, particleAmount),
			BaseUpdaterComponent(gameObject), UpdaterComponent(gameObject), Component(gameObject),
			isRunning(false), delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0), mutex(),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{
		}

		ParticleSystemComponent::ParticleSystemComponent(
			System::GameObject* gameObject,
			unsigned int particleAmount,
			const Render::Texture2D& texture,
			const ParticleSystemSettings& systemSettings,
			ParticleSettings* particleSettings,
			int renderLayer)
			: RendererComponent(gameObject, renderLayer, particleAmount, systemSettings, texture),
			BaseRendererComponent(gameObject, renderLayer, particleAmount, systemSettings, texture),
			BaseUpdaterComponent(gameObject), UpdaterComponent(gameObject), Component(gameObject),
			isRunning(false), delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0), mutex(),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{

		}

		ParticleSystemComponent::~ParticleSystemComponent()
		{
			if (IsRunning())
			{
				Stop();
			}
			if (particleSettings != nullptr)
			{
				delete particleSettings;
			}
		}

		void ParticleSystemComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Init();
				UpdaterComponent::Init();
				++ParticleSystemComponent::refrenceNumber;
			}
			else
			{
				RendererComponent::Init();
				UpdaterComponent::Init();
				++ParticleSystemComponent::refrenceNumber;
			}
		}

		void ParticleSystemComponent::DestroyObject()
		{
			if (IsRunning())
			{
				Stop();
			}

			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Destroy();
				UpdaterComponent::Destroy();
				if (!(--ParticleSystemComponent::refrenceNumber))
				{
					ParticleRenderer::GetInstance().Destroy();
					ComponentManager::GetInstance().RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				RendererComponent::Destroy();
				UpdaterComponent::Destroy();
				if (!(--ParticleSystemComponent::refrenceNumber))
				{
					ParticleRenderer::GetInstance().Destroy();
					ComponentManager::GetInstance().RemoveRendererFromRender(GetId());
				}
			}

		}

		void ParticleSystemComponent::Destroy()
		{
			if (ComponentManager::GetInstance().GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				DestroyObject();
			}
			else
			{
				DestroyObject();
			}
		}

		void ParticleSystemComponent::Update()
		{

			if (!isRunning)
				return;

			if (delayTime > 0.0f)
			{
				delayTime -= Game::GetDeltaTime();
				return;
			}

			data.isRenderable = isRunning && delayTime <= 0.0f;

			UpdateActiveParticles();
			TryToSpawnNewParticles();
		}

		const std::string& ParticleSystemComponent::GetId() const
		{
			return ParticleSystemComponent::id;
		}

		ParticleRenderer* ParticleSystemComponent::GetRenderer() const
		{
			auto& renderer = ParticleRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void ParticleSystemComponent::UpdateActiveParticles()
		{
			for (int i = 0; i < data.GetParticleAmount(); ++i)
			{
				data.particles[i].lifeTime -= Game::GetDeltaTime();
				if (data.particles[i].lifeTime > 0.0f)
				{
					particleSettings->UpdateParticle(data.particles[i], *gameObject);
				}
			}
		}

		void ParticleSystemComponent::TryToSpawnNewParticles()
		{
			nextSpawnTime -= Game::GetDeltaTime();

			float spawnTime = data.systemSettings.spawnTime < 0.0f ? 0.0f : data.systemSettings.spawnTime;

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

		void ParticleSystemComponent::SpawnNewParticles()
		{
			for (int i = 0; i < data.systemSettings.newParticlesPerSpawn; ++i)
			{
				unsigned int index = GetUnusedParticleIndex();
				particleSettings->SpawnParticle(data.particles[index], *gameObject);
			}
		}

		unsigned int ParticleSystemComponent::GetUnusedParticleIndex()
		{
			for (unsigned int i = lastUsedParticleIndex; i < data.GetParticleAmount(); ++i) {
				if (data.particles[i].lifeTime <= 0.0f) {
					lastUsedParticleIndex = i;
					return i;
				}
			}

			for (unsigned int i = 0; i < lastUsedParticleIndex; ++i) {
				if (data.particles[i].lifeTime <= 0.0f) {
					lastUsedParticleIndex = i;
					return i;
				}
			}

			//It has to have something. So this is the default.
			lastUsedParticleIndex = 0;
			return 0;
		}

		void ParticleSystemComponent::Start()
		{
			particleSettings->Init(*gameObject);

			delayTime = data.systemSettings.delayBeforeStart;
			nextSpawnTime = 0.0f;
			lastUsedParticleIndex = 0;
			isRunning = true;
		}

		void ParticleSystemComponent::Stop()
		{
			particleSettings->Destroy();
			isRunning = false;
			data.isRenderable = false;
		}
	}
}