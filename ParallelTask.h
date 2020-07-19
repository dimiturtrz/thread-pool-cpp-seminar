#pragma once

#include <thread>
#include "Task.h"
#include "PartitionTask.h"
#include "ThreadPool.h"

template<typename A, typename P>
class ParallelTask: public Task {
public:
	ParallelTask(ThreadPool& threadPool, A& indexableCollection, size_t size, size_t desiredDivision = 0):
		threadPool(threadPool), indexableCollection(indexableCollection), size(size), desiredDivision(desiredDivision){}

	virtual void execute() final;
	virtual Task* clone();
private:

	ThreadPool& threadPool;
	A& indexableCollection;
	size_t size;
	size_t desiredDivision;
};

template<typename A, typename P>
inline void ParallelTask<A, P>::execute() {
	desiredDivision = (desiredDivision != 0) ? desiredDivision : std::thread::hardware_concurrency();
	size_t chunkSize = size / (desiredDivision + (size % desiredDivision));
	for (size_t i = 0; i < desiredDivision; ++i) {
		P* partialTask = new P(
			indexableCollection,
			chunkSize * i,
			std::min(chunkSize * (i + 1), size)
		);
		threadPool.addWork(dynamic_cast<Task*>(partialTask));
	}
}

template<typename A, typename P>
inline Task* ParallelTask<A, P>::clone() {
	return new ParallelTask<A, P>(*this);
}
