#include "iscript.h"
#include "scriptregistry.h"

#include "components.h"
#include "Reflection/base.h"
using namespace FlexEngine;

#include <iostream>

class ComponentTestScript : public Script
{
public:
  ComponentTestScript()
  {
    ScriptRegistry::RegisterScript(this);
  }

  void Start() override
  {
    std::cout << "ComponentTest: Start" << std::endl;

    Vec2 pos(1.0f, 2.0f);
    pos.Print();
    pos.Invert();
    pos.Print();

    Reflection::TypeDescriptor* type = Reflection::TypeResolver<Vec2>::Get();
    std::cout << "Type: " << type->ToString() << std::endl;
    type->Dump(&pos);
  }

  void Stop() override
  {
    std::cout << "ComponentTest: Stop" << std::endl;
  }

  std::string GetName() const override
  {
    return "ComponentTest";
  }
};

// Static instance to ensure registration
static ComponentTestScript ComponentTest;
