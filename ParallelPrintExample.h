#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelPrintTask : public PartitionTask {

public:
	PartialParallelPrintTask(std::vector<int>& vec, size_t startIndex = 0, size_t endIndex = 0) :
		PartitionTask(startIndex, endIndex), vec(vec) {
	}

	virtual void execute(size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			std::cout << vec[i] << std::endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelPrintTask(*this);
	}

private:
	std::vector<int>& vec;
};

class ParallelPrintTask : public ParallelTask {

public:
	ParallelPrintTask(std::vector<int>& vec, ThreadPool& threadPool, size_t size, size_t desiredDivision = 0) :
		ParallelTask(threadPool, size, desiredDivision), vec(vec) {
	}

	PartitionTask* constructPartialTask(size_t startIndex, size_t endIndex) {
		return new PartialParallelSqrtTask(startIndex, endIndex);
	}

	Task* clone() { return new ParallelPrintTask(*this); }

private:
	std::vector<int>& vec;
};