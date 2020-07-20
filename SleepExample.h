#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include "Task.h"
#include "ParallelTask.h"

using namespace std;

namespace sleepExample {
	class SleepTask : public Task {
	public:
		SleepTask() {}

		virtual void execute() {
			std::this_thread::sleep_for(2ms);
		}

		virtual Task* clone() {
			return new SleepTask(*this);
		}
	};
}