#pragma once
#include"Task.h"

class ParallelTask;

class PartitionTask : public Task {
public:
	PartitionTask(size_t startIndex, size_t endIndex) :
		startIndex(startIndex), endIndex(endIndex) {}

	virtual void execute(size_t startIndex, size_t endIndex) = 0;
	virtual void execute() final;

	friend class ParallelTask;

private:
	size_t startIndex;
	size_t endIndex;
};