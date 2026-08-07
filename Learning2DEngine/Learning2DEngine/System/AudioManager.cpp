#include "AudioManager.h"

namespace Learning2DEngine
{

    namespace System
    {
        AudioManager::AudioManager()
			: isInited(false), engine()
        {

        }

        ma_engine* AudioManager::Init(const ma_engine_config& config)
        {
			if (isInited)
			{
				L2DE_LOG_WARNING("AudioManager::Init: AudioManager is already initialized and it will not be re-initialized")
				return &engine;
			}

			ma_result result = ma_engine_init(&config, &engine);
			if (result != MA_SUCCESS)
			{
				L2DE_LOG_ERROR("AudioManager::Init: Failed to initialize audio engine")
				return nullptr;
			}

			isInited = true;
			return &engine;
        }

		void  AudioManager::Terminate()
		{
			if (!isInited)
				return;

			ma_engine_uninit(&engine);
			isInited = false;
		}
    }
}