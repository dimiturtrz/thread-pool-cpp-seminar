#include "ThreadPool.h"

#include <iterator>

ThreadPool::ThreadPool(size_t threads): running(true) {
	size_t threadCount = (threads > 0) ? threads : std::thread::hardware_concurrency();
	this->threads.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		this->threads.push_back(std::thread( [this, i]() {
			boost::timer::cpu_timer threadTimer;
			threadTimer.start();
			while (this->running || !waitingJobs.empty()) {
				this->queueLock.lock();
				if (waitingJobs.empty()) {
					this->queueLock.unlock();
					continue;
				}
				std::function<void()> newJob = std::move(this->waitingJobs.front());
				this->waitingJobs.pop();
				this->queueLock.unlock();
				newJob();
			}

			threadTimer.stop();
			threadTimersLock.lock();
			threadTimers.push_back(threadTimer);
			threadTimersLock.unlock();
		} ));
	}
}

ThreadPool::~ThreadPool() {
	joinAll();
}

void ThreadPool::addWork(const std::function<void()>& newJob) {
	std::lock_guard<std::mutex> guard(queueLock);
	waitingJobs.push(newJob);
}

void ThreadPool::addWork(std::function<void()>&& newJob) {
	std::lock_guard<std::mutex> guard(queueLock);
	waitingJobs.emplace(newJob);
}

void ThreadPool::stopRunning() {
	running = false;
}

void ThreadPool::stopRunningAndJoinAll() {
	stopRunning();
	joinAll();
}

void ThreadPool::writeProfilingData(std::ostream& profilingOutputStream) const {
	for (int i = 0; i < threadTimers.size(); ++i) {
		profilingOutputStream << "thread " << i << " " << threadTimers[i].format(3) << std::endl;
	}
	profilingOutputStream << "thread pool: " << mainTimer.format(3) << std::endl;
}

void ThreadPool::joinAll() {
	for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it) {
		it->join();
	}
	threads.clear();
	mainTimer.stop();
}
