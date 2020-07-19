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
			std::lock_guard<std::mutex> writeGuard(m);
			std::cout << vec[i] << endl;
		}
	}

	virtual Task* clone() {
		return new PartialParallelPrintTask(*this);
	}
};

int main() {

	boost::timer::cpu_timer mainTimer;
	mainTimer.start();

	ThreadPool tp(10);

	for (int i = 0; i < 20; ++i) {
		Task* pt = new PrintTask(i);
		tp.addWork(pt);
		delete pt;
	}

	for (int i = 20; i < 40; ++i) {
		tp.addWork(new PrintTask(i));
	}

	std::vector<int> vec;
	for (int i = 40; i < 60; ++i) {
		vec.push_back(i);
	}

	tp.addWork(new ParallelTask<vector<int>, PartialParallelPrintTask>(tp, vec, vec.size()));

	tp.stopRunningAndJoinAll();
	tp.writeProfilingData(cout);

	return 0;
}