#include "flx_api.h"

#include "intmanager.h"

#include <iostream>

extern "C" void __FLX_API Start()
{
  #if 0
  int* object_a = new int(10);
  IntManager::SetActiveObject(object_a);
  #else
  IntManager::SetActiveObject(IntManager::CreateObject("object_a"));
  IntManager::ChangeValue(10);
  #endif

  std::cout << "Active object: " << *IntManager::GetActiveObject() << std::endl;

  #if 0
  delete object_a;
  object_a = nullptr;
  #else
  IntManager::DestroyObject("object_a");
  #endif
}
