#ifndef SINGLENODE_H
#define SINGLENODE_H

#include "Interfaces03.h"

class SingleNode :public ISingleNode03{
public:
	SingleNode() :value(0), next(NULL){}
	SingleNode(int val) :value(val), next(NULL){}
	int getValue(){ return value; }
	void setValue(int val){ value = val; }
	ISingleNode03 * getNext(){ return next; }
	void setNext(ISingleNode03 * node){ next = node; }
private:
	int value;
	ISingleNode03 * next;
};
#endif