#pragma once
#include "Interfaces03.h"


class HeapSort : public IHeapSort
{
public:
	HeapSort();
	virtual ~HeapSort();
	virtual void sort(std::vector<int>& vector);
private:
	//void max_heapify(std::vector<int>& vec, int i, int n);
	//void build_maxheap(std::vector<int>& vec, int n);
	void shift_down(std::vector<int>& heap, int i, int max);
	void to_heap(std::vector<int>& arr);
};

