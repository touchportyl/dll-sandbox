#pragma once

#include "flx_api.h"
#include "iscript.h"

#include <windows.h>

#include <unordered_map>
#include <string>
#include <functional>

// Factory type for scripts
using ScriptFactory = script_interface&(flex_interface&);
using ForceLinkFunction = void(*)();

// Section entry
struct ScriptEntry
{
  const char*       name;
  ScriptFactory*    create;
  ForceLinkFunction forceLink;
};




struct register_base
{
  const char* m_pName;
  register_base* m_pNext;
  inline static register_base* m_pHead = nullptr;
  register_base(const char* name) : m_pName(name), m_pNext(m_pHead) { m_pHead = this; }

  virtual script_interface& Create(flex_interface& Flex) = 0;
};

template< typename T>
struct registered final : register_base
{
  virtual script_interface& Create(flex_interface& Flex) { return *new T(Flex); }

  registered(const char* name) : register_base(name) {}
};

namespace ScriptReg
{
  void RegisterAllScripts(HMODULE hModule);
  script_interface* Create(flex_interface& Flex, const std::string& name);

  inline static std::unordered_map<std::string, register_base*> FactoriesV2 = {};
}




// Registry that stores factories instead of objects
class __FLX_API ScriptRegistry
{
public:
  static void RegisterFactory(const char* name, ScriptFactory* fn);
  static script_interface* Create(flex_interface& Flex, const std::string& name);
  static void Clear();
  static std::vector<std::string> GetScriptNames();

  // Utility for debugging
  static void ListAll();

  // Call this to register all scripts from the DLL
  static void RegisterAllScripts(HMODULE hModule);
private:
  static std::unordered_map<std::string, ScriptFactory*>& Factories();

public:
};


#define REGISTER_SCRIPT(ClassName) \
  extern "C" void __declspec(dllexport) Register_##ClassName() { \
    ScriptRegistry::RegisterFactory(#ClassName, [](flex_interface& Flex) -> script_interface& { return *new ClassName(Flex); }); \
  }

