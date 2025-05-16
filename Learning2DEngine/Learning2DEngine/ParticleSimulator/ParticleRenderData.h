#pragma once

#include "../Render/RenderData.h"
#include "../Render/Texture2D.h"
#include "../Render/Texture2DContainer.h"
#include "Particle.h"

#include "ParticleSystemSettings.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleSystemComponent;

		class ParticleRenderData final : public Render::RenderData, public Render::Texture2DContainer
		{
			friend class ParticleSystemComponent;
		private:
			bool isRenderable;
			const unsigned int minAllocateSize;
			const unsigned int particleAmount;
			Particle* particles;

		public:
			ParticleSystemSettings systemSettings;

			ParticleRenderData(const System::Component* component, unsigned int particleAmount, unsigned int minAllocateSize = 0)
				: RenderData(component), minAllocateSize(minAllocateSize), particleAmount(particleAmount), isRenderable(false),
				particles(nullptr), systemSettings(), Texture2DContainer()
			{
				particles = new Particle[particleAmount];
			}

			ParticleRenderData(
				const System::Component* component,
				unsigned int particleAmount,
				const ParticleSystemSettings& systemSettings,
				const Render::Texture2D& texture,
				unsigned int minAllocateSize = 0)
				: RenderData(component), minAllocateSize(minAllocateSize), particleAmount(particleAmount), isRenderable(false),
				particles(nullptr), systemSettings(systemSettings), Texture2DContainer(texture)
			{
				particles = new Particle[particleAmount];
			}

			~ParticleRenderData() override
			{
				delete[] particles;
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

			inline unsigned int GetMinAllocateSize() const
			{
				return minAllocateSize;
			}
		};
	}
}