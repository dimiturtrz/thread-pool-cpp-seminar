#include <iostream>
#include <thread>

#include "Task.h"
#include "ParallelTask.h"
#include "ThreadPool.h"

#include "SleepExample.h"
#include "PrintExample.h"
#include "ParallelSqrtExample.h"
#include "ParallelPrintExample.h"
#include "SqrtExample.h"

using namespace std;

int main() {

	boost::timer::cpu_timer mainTimer;
	mainTimer.start();

	ThreadPool tp;

	for (int i = 0; i < 1000; ++i) {
		Task* pt = new SqrtExample(i);
		tp.addWork(pt);
		delete pt;
	}

	/*for (int i = 0; i < 100000; ++i) {
		tp.addWork(new PrintTask(i));
	}*/

	/*for (int i = 0; i < 100000; ++i) {
		Task* pt = new PrintTask(i);
		//tp.addWork(pt);
		delete pt;
	}

	for (int i = 0; i < 100000; ++i) {
		tp.addWork(new PrintTask(i));
	}*/


	/*for (int i = 0; i < 1000; ++i) {
		Task* pt = new SleepTask(10);
		tp.addWork(pt);
		delete pt;
	}*/

	/*for (int i = 0; i < 1000; ++i) {
		tp.addWork(new SleepTask(10));
	}*/

	/*std::vector<int> vec;
	for (int i = 2000; i < 4000; ++i) {
		vec.push_back(i);
	}

	tp.addWork(new ParallelTask<vector<int>, PartialParallelPrintTask>(tp, vec, vec.size()));*/
	//tp.addWork(new ParallelTask<vector<int>, int>(tp, vec, vec.size()));


	/*std::vector<int> vec1;
	for (long long i = 4000; i < 5000000; ++i) {
		vec1.push_back(i);
	}
	tp.addWork(new ParallelTask<vector<int>, PartialParallelSqrtTask>(tp, vec1, vec1.size()));*/

	tp.stopRunningAndJoinAll();
	tp.writeProfilingData(cout);

	return 0;
}