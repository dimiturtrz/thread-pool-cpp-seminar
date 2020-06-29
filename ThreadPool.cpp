#include "ThreadPool.h"

#include <iterator>

ThreadPool::ThreadPool(size_t threads, std::ostream* profilingOutputStream): profilingOutputStream(nullptr), running(true) {
	size_t threadCount = (threads > 0) ? threads : std::thread::hardware_concurrency();
	this->threads.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		this->threads.push_back(std::thread( [this]() { 
			while (this->running || !waitingJobs.empty()) {
				if (waitingJobs.empty()) {
					continue;
				}
				std::function<void()> newJob = this->waitingJobs.front();
				this->waitingJobs.pop();
				newJob();
			}
		} ));
	}
}

ThreadPool::~ThreadPool() {
	for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it) {
		it->join();
	}
}

void ThreadPool::addWork(std::function<void()> newJob) {
	waitingJobs.push(newJob);
}

void ThreadPool::stopRunning() {
	running = false;
}
