#include "ThreadManager.h"

namespace Learning2DEngine
{
	namespace System
	{
        ThreadManager::ThreadManager(unsigned int threadCount)
            : threads(), jobQueue(),
            queueMutex(), cv(),
            running(true), activeJobs(0)
        {
            for (unsigned int i = 0; i < threadCount; ++i)
            {
                threads.emplace_back([this]
                    {
                        RunThread();
                    }
                );
            }
        }

        ThreadManager::~ThreadManager()
        {
            running = false;
            cv.notify_all();

            for (auto& t : threads)
            {
                if (t.joinable())
                    t.join();
            }
        }

        void ThreadManager::Enqueue(std::function<void()>&& job)
        {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                jobQueue.push(std::move(job));
                ++activeJobs;
            }

            cv.notify_one();
        }

        void ThreadManager::WaitAll()
        {
            while (activeJobs > 0)
                std::this_thread::yield();
        }

        void ThreadManager::RunThread()
        {
            while (running)
            {
                std::function<void()> job;

                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    // Wait until there is a job in the queue or the thread manager is stopped
                    cv.wait(lock, [&]
                        {
                            return !jobQueue.empty() || !running;
                        }
                    );

                    if (!running)
                        return;

                    job = std::move(jobQueue.front());
                    jobQueue.pop();
                }

                job();
                --activeJobs;
            }
        }
	}
}