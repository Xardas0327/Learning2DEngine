#pragma once

#include "../System/GameObjectManager.h"
#include "../Render/RenderData.h"
#include "../Render/Texture2D.h"
#include "../Render/Texture2DContainer.h"
#include "ParticleComponent.h"

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
			const size_t minAllocateSize;
			const size_t particleAmount;
			ParticleComponent** particles;

			void InitParticles()
			{
				auto& gameObjectManager = System::GameObjectManager::GetInstance();
				gameObjectManager.AddReserve(particleAmount);

				particles = new ParticleComponent * [particleAmount];
				for (size_t i = 0; i < particleAmount; ++i)
				{
					auto gameObject = gameObjectManager.CreateGameObject(false);
					particles[i] = gameObject->AddComponent<ParticleComponent>();
				}
			}

		public:
			ParticleSystemSettings systemSettings;
			bool isUseCameraView;

			ParticleRenderData(const System::Component* component, size_t particleAmount, bool isUseCameraView = true, size_t minAllocateSize = 0)
				: RenderData(component), minAllocateSize(minAllocateSize), particleAmount(particleAmount), isRenderable(false),
				particles(nullptr), systemSettings(), isUseCameraView(isUseCameraView), Texture2DContainer()
			{
				InitParticles();
			}

			ParticleRenderData(
				const System::Component* component,
				size_t particleAmount,
				const ParticleSystemSettings& systemSettings,
				const Render::Texture2D& texture,
				bool isUseCameraView = true,
				size_t minAllocateSize = 0)
				: RenderData(component), minAllocateSize(minAllocateSize), particleAmount(particleAmount), isRenderable(false),
				particles(nullptr), systemSettings(systemSettings), isUseCameraView(isUseCameraView), Texture2DContainer(texture)
			{
				InitParticles();
			}

			~ParticleRenderData() override
			{
				auto& gameObjectManager = System::GameObjectManager::GetInstance();
				for (size_t i = 0; i < particleAmount; ++i)
				{
					gameObjectManager.DestroyGameObject(particles[i]);
				}

				delete[] particles;
			}

			inline bool IsRenderable() const
			{
				return isRenderable;
			}

			inline ParticleComponent** GetParticles() const
			{
				return particles;
			}

			inline size_t GetParticleAmount() const
			{
				return particleAmount;
			}

			inline size_t GetMinAllocateSize() const
			{
				return minAllocateSize;
			}
		};
	}
}