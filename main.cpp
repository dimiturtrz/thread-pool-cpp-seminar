#include <iostream>
#include "ThreadPool.h"
using namespace std;

int main() {
	ThreadPool tp;
	tp.addWork([]() {
		cout << "Hello 1"<< endl;
	});
	tp.addWork([]() {
		cout << "Hello 2" << endl;
	});
	tp.addWork([]() {
		cout << "Hello 3" << endl;
	});
	tp.stopRunning();

	return 0;
}