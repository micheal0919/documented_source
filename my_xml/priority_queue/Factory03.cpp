#include "Factory03.h"


Factory03::Factory03()
{
}

Factory03::~Factory03()
{
}
 
void * Factory03::create(std::string name)
{
  if(name == "IVectorDoubleSort1"){
    return NULL;
  } else if(name == "IVectorDoubleSort2"){
    return NULL;
  } else if(name == "ILinkedListSort"){
    return NULL;
  } else if(name == "IRunningMedian"){
    return NULL;
  } else if(name == "IHeapSort"){
    return NULL;
  } else if(name == "ISingleNode03"){
    return NULL;
  } else if(name == "ICircularList"){
    return NULL;
  } else if(name == "IMergeSort"){
    return NULL;
  } else if (name == "IDoubleNode03"){
      return NULL;
  } else if (name == "IDoubleList03"){
      return NULL;
  } else if (name == "IKthMin"){
    return NULL;
  } else {
    return NULL;
  }
}

extern "C" {

ObjectFactory * createObjectFactory(){
  return new Factory03();  
}

}
