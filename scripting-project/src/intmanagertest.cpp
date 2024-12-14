#include "iscript.h"
#include "scriptregistry.h"

#include "intmanager.h"
#include "components.h"

#include <iostream>

class IntManagerTestScript : public Script
{
public:
  IntManagerTestScript()
  {
    ScriptRegistry::RegisterScript(this);
  }

  void Start() override
  {
    std::cout << "IntManager: Start" << std::endl;

    IntManager::SetActiveObject(IntManager::CreateObject("object_a"));
    IntManager::ChangeValue(10);
    std::cout << "Active object: " << *IntManager::GetActiveObject() << std::endl;
    IntManager::DestroyObject("object_a");
  }

  void Stop() override
  {
    std::cout << "IntManagerTest: Stop" << std::endl;
  }

  std::string GetName() const override
  {
    return "IntManagerTest";
  }
};

// Static instance to ensure registration
static IntManagerTestScript IntManagerTest;
