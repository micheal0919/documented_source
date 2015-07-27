#pragma once
#include "Interfaces03.h"
class KthMin :
	public IKthMin
{
public:
	KthMin();
	virtual ~KthMin();
	virtual int findKthMin(IDoubleList03 * data, int k);
};

