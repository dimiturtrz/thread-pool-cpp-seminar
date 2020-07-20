#pragma once

#include <thread>
#include "Task.h"
#include "PartitionTask.h"
#include "ThreadPool.h"

class ParallelTask: public Task {
public:
	ParallelTask(ThreadPool& threadPool, size_t size, size_t desiredDivision = 0) :
		threadPool(threadPool), size(size), desiredDivision(desiredDivision) {}

	virtual void execute() final;
	virtual PartitionTask* constructPartialTask(size_t startIndex, size_t endIndex) = 0;

private:

	ThreadPool& threadPool;
	size_t size;
	size_t desiredDivision;
};

inline void ParallelTask::execute() {
	desiredDivision = (desiredDivision != 0) ? desiredDivision : std::thread::hardware_concurrency();
	size_t chunkSize = size / (desiredDivision + (size % desiredDivision));
	PartitionTask* partialTask;
	for (size_t i = 0; i < desiredDivision; ++i) {
		partialTask = constructPartialTask(chunkSize * i, std::min(chunkSize * (i + 1), size));
		threadPool.addWork(std::move(partialTask));
	}
}
