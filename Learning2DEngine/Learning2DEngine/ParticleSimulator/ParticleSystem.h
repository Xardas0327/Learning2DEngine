#pragma once

#include "../Render/RendererComponent.h"
#include "../Render/Shader.h"
#include "../Render/Texture2D.h"
#include "../System/UpdaterComponent.h"
#include "../System/GameObject.h"
#include "ParticleSystemSettings.h"
#include "ParticleSettings.h"
#include "Particle.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		/// <summary>
		/// ParticleSystem::Update() update the particles.
		/// This should be called in the Game::Update() only ones.
		/// It and ParticleSystem::Draw() work only, when the ParticleSystem::IsRunning() is true.
		/// </summary>
		class ParticleSystem : public virtual Render::RendererComponent, public virtual System::UpdaterComponent
		{
			friend class System::GameObject;
		protected:
			bool isInit;
			bool isRunning;
			unsigned int particleAmount;
			Particle* particles;
			float delayTime;
			float nextSpawnTime;
			unsigned int lastUsedParticleIndex;

			/// <summary>
			/// It is counted, that how many ParticleSystem use shader and voa.
			/// It is important, that the shader will be created, if it is used and
			/// it will be destroyed, if nothing use it.
			/// </summary>
			static int referenceNumber;
			static Render::Shader shader;
			static unsigned int vao;
			static unsigned int vbo;
			static unsigned int ebo;

			ParticleSettings* particleSettings;

			ParticleSystem(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				ParticleSettings* particleSettings = nullptr,
				int renderLayer = 0);
			ParticleSystem(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				const Render::Texture2D& texture,
				const ParticleSystemSettings& systemSettings,
				ParticleSettings* particleSettings = nullptr,
				int renderLayer = 0);
			void Init() override;
			void Destroy() override;
			void InitShader();
			void InitVao();
			void UpdateActiveParticles();
			void TryToSpawnNewParticles();
			void SpawnNewParticles();
			unsigned int GetUnusedParticleIndex();
		public:
			ParticleSystemSettings systemSettings;
			Render::Texture2D* texture;

			~ParticleSystem();

			void Draw() override;

			void Start();
			void Update() override;
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

			inline ParticleSettings* const GetParticleSettings()
			{
				return particleSettings;
			}

			inline bool IsUseTexture()
			{
				return texture != nullptr;
			}

			void ClearTexture();
		};
	}
}