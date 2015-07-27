#ifndef DOUBLENODE_H
#define DOUBLENODE_H

#include "Interfaces03.h"

class DoubleNode :public IDoubleNode03{
public:
	DoubleNode() :value(0), prev(NULL), next(NULL){}
	DoubleNode(int val) :value(val), prev(NULL), next(NULL){}
	//~DoubleNode();
	int getValue(){ return value; }
	void setValue(int val){ value = val; }
	IDoubleNode03 * getNext(){ return next; }
	void setNext(IDoubleNode03 * node){ next = node; }
	IDoubleNode03 * getPrev(){ return prev; }
	void setPrev(IDoubleNode03 * node){ prev = node; }
private:
	int value;
	IDoubleNode03 * prev;
	IDoubleNode03 * next;
};

#endif