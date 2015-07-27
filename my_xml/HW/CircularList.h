#ifndef CIRCULARLIST_H
#define CIRCULARLIST_H

#include "Interfaces03.h"
#include "SingleNode.h"

class CircularList: public ICircularList{
public:
	CircularList():listSize(0),tail(NULL){}
	//~CircularList(){ delete tail; }
	void addHead(int number);
	ISingleNode03 * getTail(){ return tail; }
	void setTail(ISingleNode03 * node){ tail = node; }
	int size(){ return listSize; }
private:
	ISingleNode03 * tail;
	int listSize;
};

inline void CircularList::addHead(int number){
	SingleNode * newNode = new SingleNode(number);
	if (listSize == 0){
		tail = newNode;
		tail->setNext(newNode);
	}
	else{
		ISingleNode03 * head = tail->getNext();
		tail->setNext(newNode);
		newNode->setNext(head);
		tail = newNode;
	}
	listSize++;
}

#endif