#pragma once

/*#include <thread>
#include "Task.h"
#include "ThreadPool.h"

template<typename A>
class ParallelTask: public Task {
public:
	ParallelTask(ThreadPool& threadPool, A& indexableCollection, size_t size, size_t desiredDivision):
		threadPool(threadPool), indexableCollection(indexableCollection), size(size), desiredDivision(desiredDivision){}

	virtual void execute(A& indexableCollection, size_t startIndex, size_t endIndex) = 0;
	virtual void execute();
private:
	class ParallelPartitionTask: Task {
		ParallelPartitionTask(A& indexableCollection, size_t startIndex, size_t endIndex, void (*executeFunction)(A&, size_t, size_t)) :
			indexableCollection(indexableCollection), startIndex(startIndex), endIndex(endIndex), executeFunction(executeFunction) {}

		virtual void execute();
	private:
		void (*executeFunction)(A&, size_t, size_t);

		A& indexableCollection;
		size_t startIndex;
		size_t endIndex;
	};

	ThreadPool& threadPool;
	A& indexableCollection;
	size_t size;
	size_t desiredDivision;
};

template<typename A>
inline void ParallelTask<A>::execute() {

	desiredDivision = (desiredDivision != 0) ? desiredDivision : threads.size();
	size_t chunkSize = vector.size() / (desiredDivision + (vector.size() % desiredDivision));
	for (size_t i = 0; i < desiredDivision; ++i) {
		ParallelPartitionTask* partialTask = new ParallelPartitionTask(
			indexableCollection, 
			chunkSize * i, 
			std::min(chunkSize * (i + 1), size), 
			this->execute
		);
		threadPool.addWork(partialTask);
	}
}

template<typename A>
inline void ParallelTask<A>::ParallelPartitionTask::execute() {
	executeFunction(indexableCollection, startIndex, endIndex);
}*/