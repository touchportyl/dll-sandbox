#include "scriptregistry.h"

#include <iostream>
#include <cassert>

namespace ScriptRegistry
{
  IScriptBase* Create(EngineInterface& Flex, const std::string& name)
  {
    auto it = s_script_factories.find(name);
    if (it != s_script_factories.end())
    {
      return &(it->second->Create(Flex));
    }
    return nullptr;
  }

  std::vector<std::string> GetScriptNames()
  {
    std::vector<std::string> names;
    for (const auto& pair : s_script_factories)
    {
      names.push_back(pair.first);
    }
    return names;
  }

  void RegisterAllScripts(HMODULE hModule)
  {
    using func_type = ScriptRegistryNode * (void);

    // Get the get_scripts function
    auto func = reinterpret_cast<func_type*>(GetProcAddress(hModule, "get_scripts"));
    if (func == nullptr)
    {
      std::cout << "Error: get_scripts function not found in the DLL." << std::endl;
      return;
    }

    // First clear any existing registrations
    ClearFactories();

    // Iterate through the linked list of registered scripts
    for (auto p = func(); p != nullptr; p = p->m_next)
    {
      std::cout << "Registering Script: " << p->m_script_name << std::endl;
      auto it = s_script_factories.find(std::string(p->m_script_name));
      assert(it == s_script_factories.end() && "Duplicate script name in registration!");
      s_script_factories[p->m_script_name] = p;
    }
  }

  void ClearFactories()
  {
    s_script_factories.clear();
    ScriptRegistryNode::m_head = nullptr;
  }
}