#include "flx_api.h"

#include "intmanager.h"

#include <iostream>

extern "C" void __FLX_API Start()
{
  IntManager::SetActiveObject(IntManager::CreateObject("object_a"));
  IntManager::ChangeValue(10);

  std::cout << "Active object: " << *IntManager::GetActiveObject() << std::endl;

  IntManager::DestroyObject("object_a");
}
