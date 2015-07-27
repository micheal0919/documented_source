#ifndef VECTORDOUBLESORT_H
#define VECTORDOUBLESORT_H

#include "Interfaces03.h"

class VectorDoubleSort :public IVectorDoubleSort{
public:
	VectorDoubleSort(){}
	void sort(std::vector<double>& vec);
	void mergeSort(std::vector<double>& v, std::vector<double>& tmpArray, int left, int right);
	void merge(std::vector<double>& v, std::vector<double>& tmpArray, int leftPos, int rightPos, int rightEnd);
};

#endif