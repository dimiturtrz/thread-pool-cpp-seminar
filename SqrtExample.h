#pragma once

#include <iostream>

#include "Task.h"

class SqrtExample : public Task {

public:
	SqrtExample(int val): val(val) {}
	
	void execute() {
		std::cout<< std::sqrt(val);
	}

	Task* clone() {
		return new SqrtExample(*this);
	}

private:
	int val;
};
