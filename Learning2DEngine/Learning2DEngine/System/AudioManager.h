#pragma once

#include <miniaudio/miniaudio.h>

#include "Singleton.h"
#include "../DebugTool/Log.h"

namespace Learning2DEngine
{

    namespace System
    {
        class AudioManager final : public Singleton<AudioManager>
        {
            friend class Singleton<AudioManager>;
        private:
            bool isInited;
            ma_engine engine;

            AudioManager();

        public:
            ~AudioManager() = default;

			ma_engine* Init(const ma_engine_config& config = ma_engine_config_init());

			// If the AudioManager was initialized, this terminate will be called in Game::Terminate().
            void Terminate();

            inline ma_engine* GetEngine()
            {
                L2DE_LOG_IF_ERROR(!isInited, "AudioManager::GetEngine: AudioManager is not initialized")

				return &engine;
            }

			inline bool IsInited() const
			{
				return isInited;
			}
        };
    }
}