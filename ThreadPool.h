#pragma once

#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<thread>
#include <mutex>
#include <boost/timer/timer.hpp>

#include "Task.h"

class ThreadPool {
public:
	ThreadPool(size_t threads = 0);
	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool& operator=(const ThreadPool& other) = delete;
	~ThreadPool();

	void addWork(Task*& newJob);
	void addWork(Task*&& newJob);

	/*template<typename T>
	void addWork(const std::function<void(std::vector<T>&, size_t, size_t)>& newJob, std::vector<T>& vector, size_t desiredDivision = 0);
	*/

	void stopRunning();
	void stopRunningAndJoinAll();

	void writeProfilingData(std::ostream& profilingOutputStream) const;
private:
	std::vector<std::thread> threads;
	std::queue<Task*> waitingTasks;
	std::mutex queueLock;

	boost::timer::cpu_timer mainTimer;
	std::vector<boost::timer::cpu_timer> threadTimers;
	std::mutex threadTimersLock;

	bool running;

	void joinAll();
};