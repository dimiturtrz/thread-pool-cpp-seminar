#include <iostream>
#include <future>
#include "Task.h"
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

int main() {

	boost::timer::cpu_timer mainTimer;
	mainTimer.start();

	ThreadPool tp(10);

	for (int i = 0; i < 50; ++i) {
		Task* pt = new PrintTask(i);
		tp.addWork(pt);
		delete pt;
	}

	for (int i = 50; i < 100; ++i) {
		tp.addWork(new PrintTask(i));
	}

	tp.stopRunningAndJoinAll();
	tp.writeProfilingData(cout);

	return 0;
}