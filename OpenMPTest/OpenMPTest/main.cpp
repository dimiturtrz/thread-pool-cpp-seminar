#include <chrono>
#include <vector>
#include <thread>
#include <iostream>
#include <omp.h>

using namespace std;

/*
This source code can be downloaded from supercomputingblog.com
The purpose of this code is to ensure that OpenMP is working and running on multiple threads.
*/

int main(int argc, char* argv[]) {
	auto start = chrono::steady_clock::now();

	#pragma omp parallel for
	for (int i = 0; i < 1000; ++i) {
		//cout << i << endl;
		//std::this_thread::sleep_for(10ms);
		cout<< std::sqrt(i);
	}

	/*vector<int> v;
	for (int i = 2000; i < 4000; ++i) {
		v.push_back(i);
	}*/

	/*for (int i = 0; i < v.size(); ++i) {
		#pragma omp parallel
		{
			// This statement will run on each thread.
			// If there are 4 threads, this will execute 4 times in total
			cout << v[i] << endl;
		}
	}*/

	auto end = chrono::steady_clock::now();

	cout << "Elapsed time in seconds : "
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
		<< " ms" << endl;

	return 0;
}
