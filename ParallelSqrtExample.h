#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelSqrtTask : public PartitionTask {

public:
	PartialParallelSqrtTask(size_t startIndex = 0, size_t endIndex = 0) :
		PartitionTask(startIndex, endIndex) {
	}

	virtual void execute(size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			double sum = 0;
			for (int j = 0; j < 100000; ++j) {
				sum += std::sqrt(i);
			}
			std::cout << sum<< std::endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelSqrtTask(*this);
	}
};

class ParallelSqrtTask : public ParallelTask {

public:
	using ParallelTask::ParallelTask;

	PartitionTask* constructPartialTask(size_t startIndex, size_t endIndex) { 
		return new PartialParallelSqrtTask(startIndex, endIndex);
	}

	Task* clone() { return new ParallelSqrtTask(*this); }
};