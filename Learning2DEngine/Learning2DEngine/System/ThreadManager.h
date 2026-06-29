#pragma once

#include <vector>
#include <queue>
#include <functional>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace Learning2DEngine
{
	namespace System
	{
		class ThreadManager final
		{
		private:
			std::vector<std::thread> threads;
			std::queue<std::function<void()>> jobQueue;

			std::mutex queueMutex;
			std::condition_variable cv;

			std::atomic<bool> running;
			std::atomic<int> activeJobs;

            void RunThread();

		public:
            ThreadManager(unsigned int threadCount);

            ~ThreadManager();

			void Enqueue(std::function<void()>&& job);

			void WaitAll();
		};
	}
}