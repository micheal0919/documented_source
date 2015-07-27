#pragma once

#include "ObjectFactory.h"

class Factory03 : public ObjectFactory {
public:
  Factory03();
  virtual ~Factory03();
  virtual void * create(std::string interface_name);
};
