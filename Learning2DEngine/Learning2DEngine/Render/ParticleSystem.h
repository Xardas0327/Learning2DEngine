#pragma once

#include "Renderer.h"
#include "Particle.h"
#include "Shader.h"
#include "Texture2D.h"
#include "ParticleSystemSettings.h"
#include "IParticleSettings.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// ParticleSystem::Update() the particles.
		/// This should be called in the Game::Update() only ones.
		/// It works only, when the ParticleSystem::Start() was called until the ParticleSystem::Stop() is called.
		/// When the ParticleSystem::Start() was called, the ParticleSystem will save/copy the IParticleSettings.
		/// So if the IParticleSettings is changed after the ParticleSystem::Start(), the changes won't be apply until ParticleSystem::Restart()
		/// or ParticleSystem::Stop() and ParticleSystem::Start() again.
		/// </summary>
		class ParticleSystem : public virtual Renderer
		{
			friend class System::GameObject;
		protected:
			bool isInit;
			bool isRun;
			unsigned int particleAmount;
			Particle* particles;
			Texture2D texture;

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

			void InitShader();
			void InitVao();
			void UpdateActiveParticles();
			unsigned int GetUnusedParticleIndex();
			void SpawnNewParticles();
		public:
			ParticleSystemSettings systemSettings;

			ParticleSystem(System::GameObject* gameObject, const Texture2D& texture, unsigned int particleAmount);
			ParticleSystem(
				System::GameObject* gameObject,
				const Texture2D& texture,
				unsigned int particleAmount,
				const ParticleSystemSettings& systemSettings);
			ParticleSystem(
				System::GameObject* gameObject,
				const Texture2D& texture,
				unsigned int particleAmount,
				const IParticleSettings* const particleSettings);
			ParticleSystem(
				System::GameObject* gameObject,
				const Texture2D& texture,
				unsigned int particleAmount,
				const ParticleSystemSettings& systemSettings,
				const IParticleSettings* const particleSettings);
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

			IParticleSettings* const GetParticleSettings();
			void SetParticleSettings(const IParticleSettings* const particleSettings);
		};
	}
}