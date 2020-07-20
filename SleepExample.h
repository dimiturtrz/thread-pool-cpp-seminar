#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include "Task.h"
#include "ParallelTask.h"

using namespace std;

class SleepTask : public Task {
public:
	SleepTask(int sec): sec(sec) {}

	virtual void execute() {
		std::this_thread::sleep_for(std::chrono::milliseconds(sec));
	}

	virtual Task* clone() {
		return new SleepTask(*this);
	}

	int sec;
};