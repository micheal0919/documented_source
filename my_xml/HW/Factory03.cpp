#include "Factory03.h"
#include "VectorDoubleSort.h"
#include "LinkedListSort.h"
#include "RunningMedian.h"
#include "HeapSort.h"
#include "SingleNode.h"
#include "CircularList.h"
#include "MergeSort.h"
#include "DoubleNode.h"
#include "DoubleList.h"
#include "KthMin.h"


Factory03::Factory03()
{
}

Factory03::~Factory03()
{
}
 
void * Factory03::create(std::string name)
{
  if(name == "IVectorDoubleSort1"){
    return new VectorDoubleSort();
  } else if(name == "IVectorDoubleSort2"){
    return new VectorDoubleSort();
  } else if(name == "ILinkedListSort"){
    return new LinkedListSort();
  } else if(name == "IRunningMedian"){
    return new RunningMedian();
  } else if(name == "IHeapSort"){
    return new HeapSort();
  } else if(name == "ISingleNode03"){
    return new SingleNode();
  } else if(name == "ICircularList"){
    return new CircularList();
  } else if(name == "IMergeSort"){
    return new MergeSort();
  } else if (name == "IDoubleNode03"){
      return new DoubleNode();
  } else if (name == "IDoubleList03"){
      return new DoubleList();
  } else if (name == "IKthMin"){
    return new KthMin();
  } else {
    return NULL;
  }
}

extern "C" {

ObjectFactory * createObjectFactory(){
  return new Factory03();  
}

}
