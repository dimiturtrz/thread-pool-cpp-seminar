#pragma once

#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<thread>
#include <mutex>


class ThreadPool {
public:
	ThreadPool(size_t threads = 0, std::ostream* profilingOutputStream = nullptr);
	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool& operator=(const ThreadPool& other) = delete;
	~ThreadPool();

	void addWork(const std::function<void()>& newJob);
	void addWork(std::function<void()>&& newJob);
	template<typename T>
	void addWork(std::function<void(std::vector<T>)> newJob, std::vector<T> vector, size_t desiredDivision);

	void stopRunning();
private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> waitingJobs;
	std::ostream* profilingOutputStream;
	bool running;

	std::mutex queueLock;
};

template<typename T>
inline void ThreadPool::addWork(std::function<void(std::vector<T>)> newJob, 
								std::vector<T> vector, 
								size_t desiredDivision) {
	// todo
}
