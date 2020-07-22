#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelSqrtTask : public PartitionTask {

public:
	PartialParallelSqrtTask(size_t startIndex = 0, size_t endIndex = 0, int M = 100) :
		PartitionTask(startIndex, endIndex), M(M) {
	}

	virtual void execute(size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			double sum = 0;
			for (int j = 0; j < M; ++j) {
				sum += std::sqrt(i);
			}
			std::cout << sum<< std::endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelSqrtTask(*this);
	}

private:
	int M;
};

class ParallelSqrtTask : public ParallelTask {

public:
	//using ParallelTask::ParallelTask;
	ParallelSqrtTask(ThreadPool& threadPool, size_t size, int M, size_t desiredDivision = 0) :
		ParallelTask(threadPool, size, desiredDivision), M(M) {
	}

	PartitionTask* constructPartialTask(size_t startIndex, size_t endIndex) { 
		return new PartialParallelSqrtTask(startIndex, endIndex, M);
	}

	Task* clone() { return new ParallelSqrtTask(*this); }
private:
	int M;
};