#include <iostream>
#include <future>
#include "Task.h"
#include "ParallelTask.h"
#include "ThreadPool.h"
using namespace std;

std::mutex m;

class PrintTask: public Task {
public:
	PrintTask(int val) : val(val) {}

	virtual void execute() {
		std::lock_guard<std::mutex> writeGuard(m);
		std::cout << val << endl;
	}

	virtual Task* clone() {
		return new PrintTask(*this);
	}
private:
	int val;
};

class PartialParallelPrintTask : public PartitionTask<vector<int>> {
	using PartitionTask<vector<int>>::PartitionTask;

	virtual void execute(vector<int>& vec, size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			//std::lock_guard<std::mutex> writeGuard(m);
			std::cout << vec[i] << endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelPrintTask(*this);
	}
};


class PartialParallelSqrtTask : public PartitionTask<vector<int>> {
	using PartitionTask<vector<int>>::PartitionTask;

	virtual void execute(vector<int>& vec, size_t startIndex, size_t endIndex) {
		for (size_t i = startIndex; i < endIndex; ++i) {
			//std::lock_guard<std::mutex> writeGuard(m);
			//std::cout << vec[i] << endl;
			vec[i] = sqrt(vec[i]);
		}
	}

	virtual Task* clone() {
		return new PartialParallelSqrtTask(*this);
	}
};

int main() {

	boost::timer::cpu_timer mainTimer;
	mainTimer.start();

	ThreadPool tp(50);

	for (int i = 0; i < 1000; ++i) {
		Task* pt = new PrintTask(i);
		tp.addWork(pt);
		delete pt;
	}

	for (int i = 1000; i < 2000; ++i) {
		tp.addWork(new PrintTask(i));
	}

	std::vector<int> vec;
	for (int i = 2000; i < 4000; ++i) {
		vec.push_back(i);
	}

	tp.addWork(new ParallelTask<vector<int>, PartialParallelPrintTask>(tp, vec, vec.size()));
	//tp.addWork(new ParallelTask<vector<int>, int>(tp, vec, vec.size()));


	std::vector<int> vec1;
	for (long long i = 4000; i < 5000000; ++i) {
		vec1.push_back(i);
	}
	tp.addWork(new ParallelTask<vector<int>, PartialParallelSqrtTask>(tp, vec1, vec1.size()));

	tp.stopRunningAndJoinAll();
	tp.writeProfilingData(cout);

	return 0;
}