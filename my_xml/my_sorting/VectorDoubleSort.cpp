#include "VectorDoubleSort.h"

using std::vector;

void VectorDoubleSort::sort(vector<double>& vec){
	vector<double> tmpArray(vec.size());
	mergeSort(vec, tmpArray, 0, vec.size() - 1);
}

void VectorDoubleSort::mergeSort(std::vector<double>& v, std::vector<double>& tmpArray, int left, int right){
	if (left < right){
		int center = (left + right) / 2;
		mergeSort(v, tmpArray, left, center);
		mergeSort(v, tmpArray, center + 1, right);
		merge(v, tmpArray, left, center + 1, right);
	}
}

void VectorDoubleSort::merge(std::vector<double>& v, std::vector<double>& tmpArray, int leftPos, int rightPos, int rightEnd){
	int leftEnd = rightPos - 1;
	int tmp = leftPos;
	int num = rightEnd - leftPos + 1;

	while (leftPos <= leftEnd&&rightPos <= rightEnd){
		if (v[leftPos] <= v[rightPos])
			tmpArray[tmp++] = v[leftPos++];
		else
			tmpArray[tmp++] = v[rightPos++];
	}

	while (leftPos <= leftEnd)
		tmpArray[tmp++] = v[leftPos++];

	while (rightPos <= rightEnd)
		tmpArray[tmp++] = v[rightPos++];

	for (int i = 0; i < num; i++, rightEnd--)
		v[rightEnd] = tmpArray[rightEnd];
}