#pragma once

#include "../System/GameObject.h"
#include "Particle.h"

namespace Learning2DEngine
{
    namespace Render
    {
        struct IParticleSettings
        {
            virtual ~IParticleSettings() {};

            //It will run in the ParticleSystem::Start()
            virtual void Init(const System::GameObject& gameObject) {};
            //It will run in the ParticleSystem::Stop()
            virtual void Destroy() {};

            //It will run in the ParticleSystem::Update()
            virtual void SpawnParticle(Particle& particle, const System::GameObject& gameObject) = 0;
            /// <summary>
            /// It will run in the ParticleSystem::Update(),
            /// if the ParticleSystem::lifeTime is bigger than 0, after it was decreased.
            /// </summary>
            virtual void UpdateParticle(Particle& particle, const System::GameObject& gameObject) = 0;
        };
    }
}