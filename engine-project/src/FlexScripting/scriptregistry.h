#pragma once

#include "flx_api.h"
#include "iscript.h"

#include <windows.h>

#include <unordered_map>
#include <string>
#include <functional>

// Factory type for scripts
using ScriptFactory = Script * (*)();
using ForceLinkFunction = void(*)();

// Section entry
struct ScriptEntry
{
  const char* name;
  ScriptFactory create;
  ForceLinkFunction forceLink;
};

// Registry that stores factories instead of objects
class __FLX_API ScriptRegistry
{
public:
  static void RegisterFactory(const char* name, ScriptFactory fn);
  static Script* Create(const std::string& name);
  static void Clear();
  static std::vector<std::string> GetScriptNames();

  // Utility for debugging
  static void ListAll();

  // Call this to register all scripts from the DLL
  static void RegisterAllScripts(HMODULE hModule);
private:
  static std::unordered_map<std::string, ScriptFactory>& Factories();
};

#define REGISTER_SCRIPT(ClassName) \
  extern "C" void __declspec(dllexport) Register_##ClassName() { \
    ScriptRegistry::RegisterFactory(#ClassName, []() -> Script* { return new ClassName(); }); \
  }
