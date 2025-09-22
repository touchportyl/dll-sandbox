#pragma once

#include "flx_api.h"
#include "iscript.h"

#include <windows.h>

#include <unordered_map>
#include <string>
#include <functional>

// Node of a linked list of registered scripts
struct register_base
{
  const char* m_pName;
  register_base* m_pNext;
  inline static register_base* m_pHead = nullptr;
  register_base(const char* name) : m_pName(name), m_pNext(m_pHead) { m_pHead = this; }

  // Create an instance of the script class
  virtual script_interface& Create(flex_interface& Flex) = 0;
};

// Template to register a script class T
template <typename T>
struct registered final : register_base
{
  registered(const char* name) : register_base(name) {}

  // Create an instance of the script class T
  virtual script_interface& Create(flex_interface& Flex) { return *new T(Flex); }
};

namespace ScriptReg
{
  void RegisterAllScripts(HMODULE hModule);
  script_interface* Create(flex_interface& Flex, const std::string& name);

  inline static std::unordered_map<std::string, register_base*> Factories = {};
}

// Used by the script DLL to register scripts
#define REGISTER_SCRIPT(ClassName) \
  static registered<ClassName> Register(#ClassName)
