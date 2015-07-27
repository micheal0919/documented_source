#ifndef LINKEDLISTSORT_H
#define LINKEDLISTSORT_H

#include "Interfaces03.h"

class LinkedListSort: public ILinkedListSort{
public:
	LinkedListSort(){}
	LinkedListNode * sort(LinkedListNode * list);
	void mergeSort(LinkedListNode** headRef);
	void split(LinkedListNode * source, LinkedListNode** front, LinkedListNode** back);
	LinkedListNode* merge(LinkedListNode* a, LinkedListNode* b);
};

#endif