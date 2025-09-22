#include "scriptregistry.h"

#include <iostream>
#include <cassert>

namespace ScriptReg
{
  script_interface* Create(flex_interface& Flex, const std::string& name)
  {
    auto it = Factories.find(name);
    if (it != Factories.end())
    {
      return &(it->second->Create(Flex));
    }
    return nullptr;
  }

  void RegisterAllScripts(HMODULE hModule)
  {
    using type = register_base * (void);

    // Get the get_scripts function
    auto pFunc = reinterpret_cast<type*>(GetProcAddress(hModule, "get_scripts"));
    if( pFunc == nullptr)
    {
      printf("Error: get_scripts function not found in the DLL.\n");
      return;
    }

    Factories.clear();
    for (auto p = pFunc(); p != nullptr; p = p->m_pNext)
    {
      printf("Adding Script %s\n", p->m_pName);
      auto it = Factories.find(std::string(p->m_pName));
      assert(it == Factories.end() && "Duplicate script name in registration!");
      Factories[p->m_pName] = p;
    }
  }

}
