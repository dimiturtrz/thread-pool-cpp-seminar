#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelPrintTask : public PartitionTask<vector<int>> {
	using PartitionTask<vector<int>>::PartitionTask;

	virtual void execute(vector<int>& vec, size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			std::cout << vec[i] << std::endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelPrintTask(*this);
	}
};