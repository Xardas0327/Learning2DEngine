#pragma once

#include <thread>

#include "EngineMacro.h"
#include "GameObject.h"
#include "BaseComponentHandler.h"
#include "BaseUpdaterComponent.h"

namespace Learning2DEngine
{
	namespace System
	{
		class UpdaterComponentHandler : public virtual BaseComponentHandler<BaseUpdaterComponent>
		{
		private:
			/// <param name="startIndex">Inclusive</param>
			/// <param name="endIndex">Exclusive</param>
			void RunPart(size_t startIndex, size_t endIndex)
			{
				for (size_t i = startIndex; i < endIndex; ++i)
				{
					//A GameObject will only be destroyed at the end of the frame.
					if (components[i]->isActive && components[i]->gameObject->isActive)
						components[i]->Update();
				}
			}
		public:
			UpdaterComponentHandler()
				: BaseComponentHandler<BaseUpdaterComponent>()
			{

			}

			void Run() override
			{
				RefreshComponents();
#if L2DE_UPDATE_MAX_COMPONENT_PER_THREAD > 0
				if (components.size() < L2DE_UPDATE_MAX_COMPONENT_PER_THREAD)
				{
					RunPart(0, components.size());
					return;
				}

				// -1, because one thread is the main thread.
				size_t threadNumber = components.size() / L2DE_UPDATE_MAX_COMPONENT_PER_THREAD -1;

				//If remainder is not 0.
				if (components.size() % L2DE_UPDATE_MAX_COMPONENT_PER_THREAD)
				{
					++threadNumber;
				}
				std::vector<std::thread> threads;
				threads.reserve(threadNumber);

				size_t i = 0;
				for (; i < threadNumber * L2DE_UPDATE_MAX_COMPONENT_PER_THREAD; i += L2DE_UPDATE_MAX_COMPONENT_PER_THREAD)
				{
					threads.emplace_back(&UpdaterComponentHandler::RunPart, this, i, (i + L2DE_UPDATE_MAX_COMPONENT_PER_THREAD));
				}

				RunPart(i, components.size());

				for(std::thread& t : threads)
				{
					t.join();
				}
#else
				RunPart(0, components.size());
#endif
			}
		};
	}
}