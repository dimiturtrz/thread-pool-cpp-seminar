#include "ThreadPool.h"

#include <iterator>

ThreadPool::ThreadPool(size_t threads, std::ostream* profilingOutputStream): profilingOutputStream(nullptr), running(true) {
	size_t threadCount = (threads > 0) ? threads : std::thread::hardware_concurrency();
	this->threads.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		this->threads.push_back(std::thread( [this]() { 
			while (this->running || !waitingJobs.empty()) {
				try {
					this->queueLock.lock();
					if (waitingJobs.empty()) {
						this->queueLock.unlock();
						continue;
					}
					std::function<void()> newJob = this->waitingJobs.front();
					this->waitingJobs.pop();
					this->queueLock.unlock();
					newJob();
				} catch (std::system_error err) {
					std::cout << 1;
				}
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
	queueLock.lock();
	waitingJobs.push(newJob);
	queueLock.unlock();
}

void ThreadPool::stopRunning() {
	running = false;
}
