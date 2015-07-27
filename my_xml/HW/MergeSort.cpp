#include "MergeSort.h"

void MergeSort::mergeSort(ICircularList * input){
	//Convert circular list into a standard single linked list
	ISingleNode03* listHead = input->getTail()->getNext();
	input->getTail()->setNext(NULL);
	RecMergeSort(&listHead);

	//After sorting, reset to circular list
	ISingleNode03* t = listHead;
	ISingleNode03* t_prev = listHead;
	while (t != NULL){
		t_prev = t;
		t = t->getNext();
	}
	input->setTail(t_prev);
	t_prev->setNext(listHead);
}

void MergeSort::RecMergeSort(ISingleNode03** headRef){
	ISingleNode03* head = *headRef;
	ISingleNode03* a;
	ISingleNode03* b;

	//Base Case
	if (head == NULL || head->getNext() == NULL)
		return;

	//Recursive Call
	split(head, &a, &b);
	RecMergeSort(&a);
	RecMergeSort(&b);
	*headRef = merge(a, b);
}

void MergeSort::split(ISingleNode03 * source, ISingleNode03** front, ISingleNode03** back){
	ISingleNode03* fast;
	ISingleNode03* slow;

	//zero element or only one element
	if (source == NULL || source->getNext() == NULL){
		*front = source;
		*back = NULL;
	}
	else{
		slow = source;
		fast = source->getNext();
		while (fast != NULL){
			fast = fast->getNext();
			if (fast != NULL){
				slow = slow->getNext();
				fast = fast->getNext();
			}
		}
		*front = source;
		*back = slow->getNext();
		slow->setNext(NULL);
	}
}

ISingleNode03* MergeSort::merge(ISingleNode03* a, ISingleNode03* b){
	ISingleNode03* result = NULL;

	//Base Case
	if (a == NULL)
		return b;
	else if (b == NULL)
		return a;

	if (a->getValue() <= b->getValue()){
		result = a;
		result->setNext(merge(a->getNext(), b));
	}
	else{
		result = b;
		result->setNext(merge(a, b->getNext()));
	}

	return result;
}