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
			bool isRenderable;
			const unsigned int particleAmount;
			Particle* particles;

		public:
			ParticleSystemSettings systemSettings;
			Render::Texture2D* texture;

			ParticleRenderData(const System::Component* component, unsigned int particleAmount)
				: RenderData(component), particleAmount(particleAmount), isRenderable(false), particles(nullptr), systemSettings(), texture(nullptr)
			{
				particles = new Particle[particleAmount];
			}

			ParticleRenderData(
				const System::Component* component,
				unsigned int particleAmount,
				const ParticleSystemSettings& systemSettings,
				const Render::Texture2D& texture)
				: RenderData(component), particleAmount(particleAmount), isRenderable(false), particles(nullptr),
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