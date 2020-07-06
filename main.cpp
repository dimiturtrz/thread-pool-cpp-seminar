#include <iostream>
#include "ThreadPool.h"
using namespace std;

std::mutex m;

void print1() {
	std::lock_guard<std::mutex> writeGuard(m);
	std::cout << 1 << endl;
}

int main() {

	boost::timer::cpu_timer mainTimer;
	mainTimer.start();

	ThreadPool tp(10, &cout);
	for (int i = 0; i < 3000; ++i) {
		tp.addWork([i]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			std::lock_guard<std::mutex> writeGuard(m);
			cout << "Hello "<< i<< " ";
		});
	}

	std::function<void()> tf = print1;
	std::function<void()> tf2 = print1;

	tp.addWork(tf);
	tp.addWork(tf2);
	tp.addWork(tf);
	tp.addWork(tf2);

	std::vector<int> v;
	for (int i = 0; i < 50; ++i) {
		v.push_back(i);
	}

	tp.addWork<int>([](std::vector<int>& v, size_t start, size_t end) -> void {
		for (size_t i = start; i < end; ++i) {
			v[i] *= 15;
		}
	}, v);

	tp.stopRunningAndJoinAll();

	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;

	std::thread t1([] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});
	t1.join();

	ostream* profilingOutputStream = tp.getProfilingOutputStream();
	if (profilingOutputStream != nullptr) {
		const vector<boost::timer::cpu_timer>& threadTimers = tp.getThreadTimers();
		for (int i = 0; i < threadTimers.size(); ++i) {
			*profilingOutputStream << "thread " << i << " " << threadTimers[i].format(3)<< endl;
		}
		*profilingOutputStream << "main thread: " << mainTimer.format(3) << endl;
	}

	return 0;
}