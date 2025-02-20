#pragma once

#include <thread>

#include "BaseComponentHandler.h"

namespace Learning2DEngine
{
	namespace System
	{
		template<class T>
		class ThreadComponentHandler : public virtual BaseComponentHandler<T>
		{
		protected:
			//If it is 0, the class will not use threads
			unsigned int maxComponentPerThread;

			virtual void RunPart(size_t startIndex, size_t endIndex) = 0;

			void RunOnThreads()
			{
				// -1, because one thread is the main thread.
				size_t threadNumber = this->components.size() / maxComponentPerThread - 1;

				//If remainder is not 0.
				if (this->components.size() % maxComponentPerThread)
				{
					++threadNumber;
				}
				std::vector<std::thread> threads;
				threads.reserve(threadNumber);

				size_t i = 0;
				for (; i < threadNumber * maxComponentPerThread; i += maxComponentPerThread)
				{
					threads.emplace_back(&ThreadComponentHandler::RunPart, this, i, (i + maxComponentPerThread));
				}

				RunPart(i, this->components.size());

				for (std::thread& t : threads)
				{
					t.join();
				}
			}

		public:
			ThreadComponentHandler()
				: BaseComponentHandler<T>(), maxComponentPerThread(0)
			{
			}

			virtual void Run() override
			{
				this->RefreshComponents();
				if (maxComponentPerThread == 0 || this->components.size() < maxComponentPerThread)
					RunPart(0, this->components.size());
				else
					RunOnThreads();
			}

			//If it is 0, the class will not use threads
			inline void SetMaxComponentPerThread(unsigned int value)
			{
				maxComponentPerThread = value;
			}

			//If it is 0, the class will not use threads
			inline unsigned int GetMaxComponentPerThread()
			{
				return maxComponentPerThread;
			}
		};
	}
}