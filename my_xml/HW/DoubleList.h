#ifndef DOUBLELIST_H
#define DOUBLELIST_H

#include "Interfaces03.h"
#include "DoubleNode.h"

class DoubleList :public IDoubleList03{
public:
	DoubleList():listSize(0), head(NULL), tail(NULL){}
    //~DoubleList();
	void setHead(IDoubleNode03 * node){ head = node; }
	IDoubleNode03 * getHead(){ return head; }
	void setTail(IDoubleNode03 * node){ tail = node; }
	IDoubleNode03 * getTail(){ return tail; }
	void addBack(int value);
	int size(){ return listSize; }
private:
	IDoubleNode03 * head;
	IDoubleNode03 * tail;
	int listSize;
};

inline void DoubleList::addBack(int value){
	DoubleNode * newNode = new DoubleNode(value);
	if (listSize == 0){
		head = newNode;
		tail = newNode;
	}
	else{
		tail->setNext(newNode);
		newNode->setPrev(tail);
		tail = newNode;
	}
	listSize++;
}

#endif