#include "LinkedListSort.h"

LinkedListNode * LinkedListSort::sort(LinkedListNode * list){
	mergeSort(&list);
	return list;
}

void LinkedListSort::mergeSort(LinkedListNode** headRef){
	LinkedListNode* head = *headRef;
	LinkedListNode* a;
	LinkedListNode* b;

	//Base Case
	if (head == NULL || head->next == NULL)
		return;

	//Recursive Call
	split(head, &a, &b);
	mergeSort(&a);
	mergeSort(&b);
	*headRef = merge(a, b);
}

void LinkedListSort::split(LinkedListNode * source, LinkedListNode** front, LinkedListNode** back){
	LinkedListNode* fast;
	LinkedListNode* slow;

	//zero element or only one element
	if (source == NULL || source->next == NULL){
		*front = source;
		*back = NULL;
	}
	else{
		slow = source;
		fast = source->next;
		while (fast != NULL){
			fast = fast->next;
			if (fast != NULL){
				slow = slow->next;
				fast = fast->next;
			}
		}
		*front = source;
		*back = slow->next;
		slow->next = NULL;
	}
}

LinkedListNode* LinkedListSort::merge(LinkedListNode* a, LinkedListNode* b){
	LinkedListNode* result = NULL;

	//Base Case
	if (a == NULL)
		return b;
	else if (b == NULL)
		return a;

	if (a->key <= b->key){
		result = a;
		result->next = merge(a->next, b);
	}
	else{
		result = b;
		result->next = merge(a, b->next);
	}

	return result;
}