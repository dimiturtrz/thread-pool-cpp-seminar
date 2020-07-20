#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelPrintTask : public PartitionTask {

public:
	using PartitionTask::PartitionTask;

	virtual void execute(size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			std::cout << i << std::endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelPrintTask(*this);
	}
};

class ParallelPrintTask : public ParallelTask {

public:
	using ParallelTask::ParallelTask;

	PartitionTask* constructPartialTask(size_t startIndex, size_t endIndex) { 
		return new PartialParallelPrintTask(startIndex, endIndex); 
	}

	Task* clone() { return new ParallelPrintTask(*this); }
};