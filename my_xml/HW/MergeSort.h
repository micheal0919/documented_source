#ifndef MERGESORT_H
#define MERGESORT_H

#include "Interfaces03.h"

class MergeSort :public IMergeSort{
public:
	MergeSort(){}
	void mergeSort(ICircularList * input);
	void RecMergeSort(ISingleNode03** headRef);
	void split(ISingleNode03 * source, ISingleNode03** front, ISingleNode03** back);
	ISingleNode03* merge(ISingleNode03* a, ISingleNode03* b);
};

#endif