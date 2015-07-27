#pragma once
#include "Interfaces03.h"
class RunningMedian :
	public IRunningMedian
{
public:
	RunningMedian();
	virtual ~RunningMedian();
	virtual std::vector<int> compute(std::vector<int> vec, int window_size);
	int median(std::vector<int> vec);
};

