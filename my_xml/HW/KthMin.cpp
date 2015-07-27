#include "KthMin.h"
#include <vector>
#include <iostream>
#include "HeapSort.h"

using namespace std;

KthMin::KthMin()
{
}


KthMin::~KthMin()
{
}

int KthMin::findKthMin(IDoubleList03 * data, int k)
{
	IDoubleNode03* head = data->getHead();
	std::vector<int> vec;
	while (head)
	{
		vec.push_back(head->getValue());
		head = head->getNext();
	}
	//for (size_t i = 0; i < vec.size(); i++) {
	// cout << i << "th, num is " << vec[i] << endl;
	//}

	HeapSort sorter;
	sorter.sort(vec);
	return vec[k];

	int index = 0;
	int min = -1;
	k += 1;
	for (size_t i = 0; i < k; ++i) {
		if (vec.empty()) return min;
		min = vec[0];
		for (size_t j = 0; j < vec.size(); ++j) {
			if (vec[j] < min) {
				min = vec[j];
				index = j;
			}
		}
		vec.erase(vec.begin() + index);
	}
	
	return min;
}