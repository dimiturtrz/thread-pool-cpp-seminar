#include <iostream>
#include "ThreadPool.h"
using namespace std;

std::mutex m;

void print1() {
	std::lock_guard<std::mutex> writeGuard(m);
	std::cout << 1 << endl;
}

int main() {
	ThreadPool tp;
	for (int i = 0; i < 100; ++i) {
		tp.addWork([i]() {
			std::lock_guard<std::mutex> writeGuard(m);
			cout << "Hello "<< i << endl;
		});
	}

	std::function<void()> tf = print1;
	std::function<void()> tf2 = print1;

	tp.addWork(tf);
	tp.addWork(tf2);
	tp.addWork(tf);
	tp.addWork(tf2);

	std::vector<int> v;
	for (int i = 0; i < 10; ++i) {
		v.push_back(i);
	}

	tp.addWork<int>([](std::vector<int>& v, size_t start, size_t end) -> void {
		for (size_t i = start; i < end; ++i) {
			v[i] *= 2;
		}
	}, v);

	tp.stopRunningAndJoinAll();

	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i] << endl;
	}

	return 0;
}