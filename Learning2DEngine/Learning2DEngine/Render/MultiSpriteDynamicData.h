#pragma once

#include "BaseColorDynamicData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct MultiSpriteDynamicData : public BaseColorDynamicData
		{
			float textureId;
			float isUseCameraView;
		};
	}
}