#include "ParallelTask.h"

void ParallelTask::execute() {
	desiredDivision = (desiredDivision != 0) ? desiredDivision : (std::thread::hardware_concurrency() - 1);
	size_t chunkSize = size / (desiredDivision + (size % desiredDivision));
	PartitionTask* partialTask;
	for (size_t i = 0; i < desiredDivision + 1; ++i) {
		partialTask = constructPartialTask(chunkSize * i, std::min(chunkSize * (i + 1), size));
		threadPool.addWork(std::move(partialTask));
	}
}
