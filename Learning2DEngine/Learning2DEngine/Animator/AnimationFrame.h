#pragma once

#include "../Render/Texture2D.h"

namespace Learning2DEngine
{
	namespace Animator
	{
		struct AnimationFrame
		{
			float time;
			Render::Texture2D* texture;
		};
	}
}