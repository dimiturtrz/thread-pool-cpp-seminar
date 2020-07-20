#pragma once

#include <iostream>

#include "Task.h"

class PrintTask : public Task {
public:
	PrintTask(int val) : val(val) {}

	virtual void execute() {
		std::cout << val << std::endl;
	}

	virtual Task* clone() {
		return new PrintTask(*this);
	}
private:
	int val;
};