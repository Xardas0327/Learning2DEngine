#pragma once

#include <mutex>
#include <string>

#include "../Render/OldRendererComponent.h"
#include "../System/GameObject.h"
#include "../System/UpdaterComponent.h"
#include "ParticleRenderer.h"
#include "ParticleRenderData.h"
#include "ParticleSettings.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleSystemComponent final : public Render::OldRendererComponent<ParticleRenderData, ParticleRenderer>,
											public System::UpdaterComponent
		{
			friend class System::GameObject;
		private:
			bool isRunning;
			float delayTime;
			float nextSpawnTime;
			unsigned int lastUsedParticleIndex;

			ParticleSettings* particleSettings;

			static const std::string id;
			/// <summary>
			/// It is counted, that how many ParticleSystem exist.
			/// </summary>
			static int refrenceNumber;
			static std::mutex mutex;

			ParticleSystemComponent(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				ParticleSettings* particleSettings = nullptr,
				unsigned int minAllocateSize = 0,
				int renderLayer = 0);
			ParticleSystemComponent(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				const Render::Texture2D& texture,
				const ParticleSystemSettings& systemSettings,
				ParticleSettings* particleSettings = nullptr,
				unsigned int minAllocateSize = 0,
				int renderLayer = 0);

			void Init() override;
			void Destroy() override;
			void Update() override;
			const std::string& GetId() const override;
			ParticleRenderer* GetRenderer() const override;

			void DestroyObject();
			void UpdateActiveParticles();
			void TryToSpawnNewParticles();
			void SpawnNewParticles();
			unsigned int GetUnusedParticleIndex();
		public:
			~ParticleSystemComponent() override;

			void Start();
			void Stop();

			inline void Restart()
			{
				Stop();
				Start();
			}

			inline bool IsRunning()
			{
				return isRunning;
			}

			inline ParticleSettings* GetParticleSettings()
			{
				return particleSettings;
			}
		};
	}
}