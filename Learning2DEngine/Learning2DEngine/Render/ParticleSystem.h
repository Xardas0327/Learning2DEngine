#pragma once

#include "Renderer.h"
#include "Particle.h"
#include "Shader.h"
#include "Texture2D.h"
#include "ParticleSystemSettings.h"
#include "IParticleSettings.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// ParticleSystem::Update() update the particles.
		/// This should be called in the Game::Update() only ones.
		/// It and ParticleSystem::Draw() work only, when the ParticleSystem::IsRunning() is true.
		/// </summary>
		class ParticleSystem : public virtual Renderer
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
			static Shader shader;
			static unsigned int vao;
			static unsigned int vbo;
			static unsigned int ebo;

			IParticleSettings* particleSettings;

			ParticleSystem(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				IParticleSettings* particleSettings = nullptr);
			ParticleSystem(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				const Texture2D& texture,
				const ParticleSystemSettings& systemSettings,
				IParticleSettings* particleSettings = nullptr);
			void InitShader();
			void InitVao();
			void UpdateActiveParticles();
			void SpawnNewParticles();
			unsigned int GetUnusedParticleIndex();
		public:
			ParticleSystemSettings systemSettings;
			Texture2D* texture;

			~ParticleSystem();

			void Init() override;
			void Destroy() override;
			void Draw() override;

			void Start();
			void Update();
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

			inline IParticleSettings* const GetParticleSettings()
			{
				return particleSettings;
			}

			inline bool IsUseTexture()
			{
				return texture != nullptr;
			}

			inline void ClearTexture()
			{
				if (IsUseTexture())
				{
					delete texture;
					texture = nullptr;
				}
			}
		};
	}
}