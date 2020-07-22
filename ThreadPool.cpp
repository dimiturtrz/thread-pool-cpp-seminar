#include "ThreadPool.h"

#include <iterator>

ThreadPool::ThreadPool(size_t threads): running(true) {
	size_t threadCount = (threads > 0) ? threads : (std::thread::hardware_concurrency() - 1);
	this->threads.reserve(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		this->threads.push_back(std::thread( [this, i]() {
			boost::timer::cpu_timer threadTimer;
			threadTimer.start();
			while (running || !waitingTasks.empty()) {
				std::unique_lock<std::mutex> queueLock(queueMutex);
				queueCV.wait(queueLock, [this]() { 
					return !running || !waitingTasks.empty(); 
					});
				if (waitingTasks.empty() && !running) {
					queueLock.unlock();
					break;
				}

				Task* newTask = this->waitingTasks.front();
				this->waitingTasks.pop();
				queueLock.unlock();
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
	{
		std::lock_guard<std::mutex> guard(queueMutex);
		waitingTasks.push(newJob->clone());
	}
	queueCV.notify_one();
}

void ThreadPool::addWork(Task*&& newJob) {
	{
		std::lock_guard<std::mutex> guard(queueMutex);
		waitingTasks.emplace(newJob);
	}
	queueCV.notify_one();
}

void ThreadPool::stopRunning() {
	running = false;
	queueCV.notify_all();
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
