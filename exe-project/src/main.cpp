#include "intmanager.h"

#include <iostream>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

int main()
{
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  IntManager int_manager;
  int_manager.Init();

  int* object_a = new int(10);

  int_manager.SetActiveObject(object_a);
  int* active_object = IntManager::GetActiveObject();
  std::cout << "Active object: " << *active_object << std::endl;
  int_manager.Shutdown();

  delete object_a;
  object_a = nullptr;

  return 0;
}