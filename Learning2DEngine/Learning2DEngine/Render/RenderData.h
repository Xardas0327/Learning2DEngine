#pragma once

#include "../System/GameObject.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct RenderData
		{
		public:
			const System::GameObject* const gameObject;

			RenderData(System::GameObject* gameObject)
				: gameObject(gameObject)
			{
			}

			virtual ~RenderData() = default;
		};
	}
}