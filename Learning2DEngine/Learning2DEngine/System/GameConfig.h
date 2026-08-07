#pragma once

#include "../Render/Resolution.h"
#include "../Render/WindowType.h"

#include "AudioManager.h"

namespace Learning2DEngine
{
    namespace System
    {
		struct GameConfig
		{
			const char* title;
			int openGLMajorVersion;
			int openGLMinorVersion;
			Render::Resolution resolution;
			Render::WindowType windowType;
			bool initAudioManager;
			ma_engine_config audioManagerConfig;

			GameConfig(
				const char* title,
				const Render::Resolution& resolution,
				int openGLMajorVersion = 3,
				int openGLMinorVersion = 3,
				Render::WindowType windowType = Render::WindowType::RESIZABLE,
				bool initAudioManager = true,
				const ma_engine_config& audioManagerConfig = ma_engine_config_init())
				: title(title), openGLMajorVersion(openGLMajorVersion), openGLMinorVersion(openGLMinorVersion),
				resolution(resolution), windowType(windowType), initAudioManager(initAudioManager), audioManagerConfig(audioManagerConfig)
			{
			
			}
		};
    }
}