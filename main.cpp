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

	tp.addWork(tf);

	tp.stopRunning();

	return 0;
}