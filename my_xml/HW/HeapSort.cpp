#include "HeapSort.h"


HeapSort::HeapSort()
{
}


HeapSort::~HeapSort()
{
}

//void HeapSort::sort(std::vector<int>& vec)
//{
//	int i, temp;
//	int n = vec.size() - 1;
//	for (i = n; i >= 2; i--)
//	{
//		temp = vec[i];
//		vec[i] = vec[1];
//		vec[1] = temp;
//		max_heapify(vec, 1, i - 1);
//	}
//}
//
//void HeapSort::max_heapify(std::vector<int>& vec, int i, int n)
//{
//	int j, temp;
//	temp = vec[i];
//	j = 2 * i;
//	while (j <= n)
//	{
//		if (j < n && vec[j + 1] > vec[j])
//			j = j + 1;
//		if (temp > vec[j])
//			break;
//		else if (temp <= vec[j])
//		{
//			vec[j / 2] = vec[j];
//			j = 2 * j;
//		}
//	}
//	vec[j / 2] = temp;
//	return;
//}
//
//void HeapSort::build_maxheap(std::vector<int>& vec, int n)
//{
//	int i;
//	for (i = n / 2; i >= 1; i--)
//	{
//		max_heapify(vec, i, n);
//	}
//}

using namespace std;

void HeapSort::shift_down(vector<int>& heap, int i, int max) {
	int i_big, c1, c2;
	while (i < max) {
		i_big = i;
		c1 = (2 * i) + 1;
		c2 = c1 + 1;
		if (c1<max && heap[c1]>heap[i_big])
			i_big = c1;
		if (c2<max && heap[c2]>heap[i_big])
			i_big = c2;
		if (i_big == i) return;
		swap(heap[i], heap[i_big]);
		i = i_big;
	}
}

void HeapSort::to_heap(vector<int>& arr) {
	int i = (arr.size() / 2) - 1;
	while (i >= 0) {
		shift_down(arr, i, arr.size());
		--i;
	}
}

void HeapSort::sort(vector<int>& arr) {
	to_heap(arr);
	int end = arr.size() - 1;
	while (end > 0) {
		swap(arr[0], arr[end]);
		shift_down(arr, 0, end);
		--end;
	}
}
