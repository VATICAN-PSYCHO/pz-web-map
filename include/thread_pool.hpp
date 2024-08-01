#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {

  public:
	explicit ThreadPool(std::uint32_t numThreads);
	~ThreadPool();

	void enqueue(std::function<void()> task);
	void wait();

  private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;

	std::mutex queueMutex;
	std::condition_variable condition;
	std::condition_variable completionCondition;
	std::atomic<bool> stop;
	std::atomic<std::uint32_t> remaningTasks;
};

#endif
