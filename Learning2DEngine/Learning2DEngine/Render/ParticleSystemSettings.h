#pragma once

#include <glad/glad.h>

#include "Blend.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct ParticleSystemSettings
		{
			bool isUseBlend;
			BlendFuncFactor blendFuncFactor;
			//Under a second
			unsigned int newParticles;
			//In seconds. The negative number is same as 0.
			float delayBeforeStart;

			ParticleSystemSettings(
				bool isUseBlend = false,
				BlendFuncFactor blendFuncFactor = BlendFuncFactor(GL_ONE, GL_ZERO),
				unsigned int newParticles = 1, float delayBeforeStart = 0.0f)
				: isUseBlend(isUseBlend), blendFuncFactor(blendFuncFactor),
				newParticles(newParticles), delayBeforeStart(delayBeforeStart)
			{

			}
		};
	}
}