#pragma once
#include"Task.h"

template<typename A>
class PartitionTask : public Task {
public:
	PartitionTask(A& indexableCollection, size_t startIndex, size_t endIndex) :
		indexableCollection(indexableCollection), startIndex(startIndex), endIndex(endIndex) {}

	virtual void execute(A& indexableCollection, size_t startIndex, size_t endIndex) = 0;
	virtual void execute() final;
private:
	A& indexableCollection;
	size_t startIndex;
	size_t endIndex;
};

template<typename A>
inline void PartitionTask<A>::execute() {
	execute(indexableCollection, startIndex, endIndex);
}