#pragma once

#include "flx_api.h"
#include "iscript.h"

#include <windows.h>

#include <unordered_map>
#include <string>
#include <functional>

// Node of a linked list of registered scripts
struct ScriptRegistryNode
{
  const char* m_script_name;
  ScriptRegistryNode* m_next;
  inline static ScriptRegistryNode* m_head = nullptr;
  ScriptRegistryNode(const char* name) : m_script_name(name), m_next(m_head) { m_head = this; }

  // Create an instance of the script class
  virtual IScriptBase& Create(EngineInterface& engine_interface) = 0;
};

// Template to register a script class T
template <typename T>
struct RegisterScript final : ScriptRegistryNode
{
  RegisterScript(const char* name) : ScriptRegistryNode(name) {}

  // Create an instance of the script class T
  virtual IScriptBase& Create(EngineInterface& engine_interface) { return *new T(engine_interface); }
};

namespace ScriptRegistry
{
  void RegisterAllScripts(HMODULE hModule);
  void ClearFactories();
  IScriptBase* Create(EngineInterface& engine_interface, const std::string& name);
  std::vector<std::string> GetScriptNames();

  inline static std::unordered_map<std::string, ScriptRegistryNode*> s_script_factories = {};
}

// Used by the script DLL to register scripts
#define REGISTER_SCRIPT(ClassName) \
  static RegisterScript<ClassName> Register(#ClassName)
