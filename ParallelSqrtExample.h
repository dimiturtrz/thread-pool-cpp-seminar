#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelSqrtTask : public PartitionTask {

public:
	PartialParallelSqrtTask(std::vector<int>& vec, size_t startIndex = 0, size_t endIndex = 0) :
		PartitionTask(startIndex, endIndex), vec(vec) {
	}

	virtual void execute(size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			std::cout << vec[i] << std::endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelSqrtTask(*this);
	}

private:
	std::vector<int>& vec;
};

class ParallelSqrtTask : public ParallelTask {

public:
	ParallelSqrtTask(std::vector<int>& vec, ThreadPool& threadPool, size_t size, size_t desiredDivision = 0) :
		ParallelTask(threadPool, size, desiredDivision), vec(vec) {
	}

	PartitionTask* constructPartialTask(size_t startIndex, size_t endIndex) { 
		return new PartialParallelSqrtTask(vec, startIndex, endIndex);
	}

	Task* clone() { return new ParallelSqrtTask(*this); }

private:
	std::vector<int>& vec;
};