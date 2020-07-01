#include <iostream>
#include "ThreadPool.h"
using namespace std;

std::mutex m;

int main() {
	ThreadPool tp;
	for (int i = 0; i < 100; ++i) {
		tp.addWork([i]() {
			std::lock_guard<std::mutex> writeGuard(m);
			cout << "Hello "<< i << endl;
		});
	}

	tp.stopRunning();

	return 0;
}