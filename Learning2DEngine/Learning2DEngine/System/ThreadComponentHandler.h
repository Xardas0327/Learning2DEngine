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
			std::vector<std::thread> threads;

			virtual void RunPart(size_t startIndex, size_t endIndex) = 0;

			void RunOnThreads()
			{
				size_t oldSize = threads.size();
				threads.clear();

				// -1, because one thread is the main thread.
				size_t threadNumber = this->components.size() / maxComponentPerThread - 1;

				//if remainder is not 0.
				if (this->components.size() % maxComponentPerThread)
				{
					++threadNumber;
				}
				
				if (oldSize > threadNumber * 2)
				{
					//if the threads vector is too big, it will be reallocated.
					threads.shrink_to_fit();
				}
				threads.reserve(threadNumber);

				for (size_t i = 0; i < threadNumber; ++i)
				{
					threads.emplace_back(&ThreadComponentHandler::RunPart, this, i * maxComponentPerThread, (i + 1) * maxComponentPerThread);
				}

				RunPart(threadNumber * maxComponentPerThread, this->components.size());

				for (std::thread& t : threads)
				{
					t.join();
				}
			}

		public:
			ThreadComponentHandler()
				: BaseComponentHandler<T>(), maxComponentPerThread(0), threads()
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