#include "ThreadPool.h"

#include <iterator>

ThreadPool::ThreadPool(size_t threads): running(true) {
	size_t threadCount = (threads > 0) ? threads : (std::thread::hardware_concurrency() - 1);
	this->threads.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		this->threads.push_back(std::thread( [this, i]() {
			boost::timer::cpu_timer threadTimer;
			threadTimer.start();
			while (this->running || !waitingTasks.empty()) {
				this->queueLock.lock();
				if (waitingTasks.empty()) {
					this->queueLock.unlock();
					continue;
				}
				Task* newTask = this->waitingTasks.front();
				this->waitingTasks.pop();
				this->queueLock.unlock();
				newTask->execute();
				delete newTask;
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

void ThreadPool::addWork(Task*& newJob) {
	std::lock_guard<std::mutex> guard(queueLock);
	waitingTasks.push(newJob->clone());
}

void ThreadPool::addWork(Task*&& newJob) {
	std::lock_guard<std::mutex> guard(queueLock);
	waitingTasks.emplace(newJob);
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
