#include "thread_pool.hpp"
#include <algorithm>
#include <stdexcept>

ThreadPool::ThreadPool(std::uint32_t numThreads) {
	this->stop.store(false);
	this->remaningTasks.store(0);

	for (std::uint32_t i = 0; i < numThreads; ++i) {
		this->workers.emplace_back([this] {
			while (true) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(this->queueMutex);
					this->condition.wait(lock, [this] {
						return this->stop.load() || !this->tasks.empty();
					});

					if (this->stop.load() && this->tasks.empty()) {
						return;
					}

					task = std::move(this->tasks.front());
					this->tasks.pop();
					this->remaningTasks++;
				}

				task();
				{
					std::unique_lock<std::mutex> lock(this->queueMutex);
					this->remaningTasks--;
					if (this->tasks.empty() && this->remaningTasks == 0) {
						this->completionCondition.notify_all();
					}
				}
			}
		});
	}
}

ThreadPool::~ThreadPool() {
	this->stop.store(true);
	this->condition.notify_all();

	for (auto &thread : this->workers) {
		thread.join();
	}
}

void ThreadPool::enqueue(std::function<void()> task) {
	{
		std::unique_lock<std::mutex> lock(this->queueMutex);

		if (this->stop.load()) {
			throw std::runtime_error("enqueue on stopped ThreadPool");
		}

		tasks.emplace(std::move(task));
	}
	condition.notify_one();
}

void ThreadPool::wait() {
	std::unique_lock<std::mutex> lock(this->queueMutex);
	this->completionCondition.wait(lock, [this] {
		return this->tasks.empty() && this->remaningTasks == 0;
	});
}
