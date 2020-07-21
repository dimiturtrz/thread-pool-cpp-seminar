#pragma once

#include <iostream>

#include "Task.h"

class SqrtExample : public Task {

public:
	SqrtExample(int val): val(val) {}
	
	void execute() {
		double sum = 0;
		for (int i = 0; i < 100000; ++i) {
			sum += std::sqrt(val);
		}
		std::cout << sum << std::endl;
	}

	Task* clone() {
		return new SqrtExample(*this);
	}

private:
	int val;
};
