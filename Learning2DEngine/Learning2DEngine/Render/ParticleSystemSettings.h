#pragma once

#include <glm/glm.hpp>

#include "Blend.h"

namespace Learning2DEngine
{
    namespace Render
    {
        enum ParticleSettingMode
        {
            //Only the start attribute will be used.
            FIX,
            //The start and end attributes will be used.
            OFFSET
        };

        template<class T>
        struct ParticleSettingRange
        {
            T min;
            T max;
        };

        struct ParticleSystemSettings
        {
            bool isUseBlend;
            BlendFuncFactor blendFuncFactor;
            //Under a second
            unsigned int newParticles;
            //In Seconds. the negative number is same as 0.
            float delayBeforeStart;
            //In Seconds. the negative number is same as 0.
            float lifeTime;

            ParticleSettingMode speedMode;
            float startSpeed;
            float endSpeed;

            ParticleSettingMode rotationMode;
            float startRotation;
            float endRotation;

            ParticleSettingMode scaleMode;
            glm::vec2 startScale;
            glm::vec2 endScale;

            ParticleSettingMode colorMode;
            glm::vec4 startColor;
            glm::vec4 endColor;

            //It will be added to the Particle::transform::position
            glm::vec2 positionStartOffset;
            //It will be added to the Particle::transform::position
            ParticleSettingRange<float> positionRandomRange;

            /// <summary>
            /// If it is true, the particle will use the GameObject::velocity
            /// with ParticleSystemSettings::useInverseVelocity.
            /// If it is false, it will use the ParticleSystemSettings::startVelocity only.
            /// </summary>
            bool isUseGameObjectVelocity;
            bool useInverseVelocity;
            glm::vec2 startVelocity;
        };
    }
}