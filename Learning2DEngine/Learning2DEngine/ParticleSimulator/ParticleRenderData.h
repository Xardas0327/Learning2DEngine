#pragma once

#include "../Render/RenderData.h"
#include "../Render/Texture2D.h"
#include "Particle.h"

#include "ParticleSystemSettings.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleSystemComponent;

		class ParticleRenderData final : public Render::RenderData
		{
			friend class ParticleSystemComponent;
		private:
			unsigned int particleAmount;
			bool isRenderable;
			//We have to init it, when we didn't gave the particleAmount
			Particle* particles;

		public:
			ParticleSystemSettings systemSettings;
			Render::Texture2D* texture;

			ParticleRenderData(System::GameObject* gameObject)
				: RenderData(gameObject), particleAmount(0), isRenderable(false), particles(nullptr), systemSettings(), texture(nullptr)
			{
			}

			ParticleRenderData(
				System::GameObject* gameObject,
				unsigned int particleAmount,
				const ParticleSystemSettings& systemSettings,
				const Render::Texture2D& texture)
				: RenderData(gameObject), particleAmount(particleAmount), isRenderable(false), particles(nullptr),
				systemSettings(systemSettings), texture(new Render::Texture2D(texture))
			{
				particles = new Particle[particleAmount];
			}

			~ParticleRenderData() override
			{
				if (IsUseTexture())
				{
					delete texture;
				}
				delete[] particles;
			}

			inline bool IsUseTexture() const
			{
				return texture != nullptr;
			}

			inline void ClearTexture()
			{
				texture = nullptr;
			}

			inline bool IsRenderable() const
			{
				return isRenderable;
			}

			inline const Particle* GetParticles() const
			{
				return particles;
			}

			inline unsigned int GetParticleAmount() const
			{
				return particleAmount;
			}
		};
	}
}