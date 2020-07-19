#pragma once
class Task {
public:
	virtual void execute() = 0;
	virtual Task* clone() = 0;

	void operator()() {
		execute();
	}
};

