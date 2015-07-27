#include "RunningMedian.h"
#include <iostream>
#include "HeapSort.h"

using namespace std;

RunningMedian::RunningMedian()
{
}


RunningMedian::~RunningMedian()
{
}

std::vector<int> RunningMedian::compute(std::vector<int> vec, int window_size)
{
	cout << __FILE__ << " : " << __LINE__ << endl;
	vector<int> res;
	vector<int> win;
	HeapSort sorter;
	for (size_t i = 0; i < 20; i++)
	{
		std::cout << "vec[] " << i << "th = " << vec[i] << std::endl;
	}

	for (size_t i = 0; i < window_size; i++) {
		win.push_back(vec[i]);
		sorter.sort(win);
		res.push_back(this->median(win));
	}
	
	cout << __FILE__ << " : " << __LINE__ << endl;
	for (size_t i = 1; i <= vec.size() - window_size; i++)
	{
		win.clear();
		//cout << __FILE__ << " : " << __LINE__ << endl;
		for (size_t j = 0; j < window_size; j++)
		{
			win.push_back(vec[i + j]);
		}
		//cout << __FILE__ << " : " << __LINE__ << endl;
		sorter.sort(win);
		//cout << __FILE__ << " : " << __LINE__ << endl;
		res.push_back(this->median(win));
	}
	cout << __FILE__ << " : " << __LINE__ << endl;
//	res.push_back(win[window_size / 2]);
	cout << "window_size = " << window_size << endl;

	return res;
}

int RunningMedian::median(std::vector<int> vec)
{
	if (vec.size() % 2 == 0)
	{
		int a = vec[vec.size() / 2];
		int b = vec[vec.size() / 2 - 1];
		return (a + b) / 2;
	}
	else
	{
		return vec[vec.size() / 2];
	}
}
