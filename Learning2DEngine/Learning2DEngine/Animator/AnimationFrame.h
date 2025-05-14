#pragma once

#include "../Render/Texture2D.h"

namespace Learning2DEngine
{
	namespace Animator
	{
		struct AnimationFrame
		{
			Render::Texture2D* texture;
			float time;
		};
	}
}