#include <iostream>
#include <future>
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

	ThreadPool tp(100);
	for (int i = 0; i < 1000; ++i) {
		tp.addWork([i]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::lock_guard<std::mutex> writeGuard(m);
			for (int j = 0; j < i; ++j) {
				sqrt(j);
			}
			//cout << "Hello "<< i<< " ";
		});
	}

	std::function<void()> tf = print1;
	std::function<void()> tf2 = print1;

	tp.addWork(tf);
	tp.addWork(tf2);
	tp.addWork(tf);
	tp.addWork(tf2);

	std::vector<int> v;
	for (int i = 0; i < 100000; ++i) {
		v.push_back(i);
	}

	tp.addWork<int>([](std::vector<int>& v, size_t start, size_t end) -> void {
		for (size_t i = start; i < end; ++i) {
			v[i] *= 15;
		}
	}, v);

	std::promise<int> p;
	std::future<int> pf = p.get_future();
	tp.addWork(std::bind([](std::promise<int>& p) { p.set_value(5 + 6); }, std::ref(p)));
	cout << pf.get() << endl;

	tp.stopRunningAndJoinAll();

	for (size_t i = 0; i < v.size(); ++i) {
		if (i % 50 == 0) {
			cout << v[i] << " ";
		}
	}
	cout << endl;

	std::thread t1([] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});
	t1.join();

	tp.writeProfilingData(cout);

	return 0;
}