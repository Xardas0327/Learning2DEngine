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

			ParticleSystemSettings()
				: isUseBlend(false), blendFuncFactor(GL_ONE, GL_ZERO),
				newParticles(1), delayBeforeStart(0.0f)
			{

			}
		};
	}
}