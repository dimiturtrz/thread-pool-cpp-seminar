#pragma once

#include <vector>
#include "ParallelTask.h"
#include "ThreadPool.h"

using namespace std;

class PartialParallelSqrtTask : public PartitionTask<vector<int>> {
	using PartitionTask<vector<int>>::PartitionTask;

	virtual void execute(vector<int>& vec, size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			vec[i] = sqrt(vec[i]);
		}
	}

	virtual Task* clone() {
		return new PartialParallelSqrtTask(*this);
	}
};