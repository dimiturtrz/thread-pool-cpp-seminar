#include <iostream>
#include "ThreadPool.h"
using namespace std;

int main() {
	ThreadPool tp;
	for (int i = 0; i < 100; ++i) {
		tp.addWork([i]() {
			cout << "Hello "<< i << endl;
		});
	}

	tp.stopRunning();

	return 0;
}