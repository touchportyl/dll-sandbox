#include "scriptregistry.h"

#include <iostream>
#include <cassert>

// Internal storage
std::unordered_map<std::string, ScriptFactory*>& ScriptRegistry::Factories()
{
  static std::unordered_map<std::string, ScriptFactory*> f;
  return f;
}

void ScriptRegistry::RegisterFactory(const char* name, ScriptFactory* fn)
{
  Factories()[name] = fn;
}

script_interface* ScriptRegistry::Create(flex_interface& Flex, const std::string& name)
{
  auto it = Factories().find(name);
  if (it != Factories().end())
  {
    return &(it->second)(Flex);
  }
  return nullptr;
}

void ScriptRegistry::Clear()
{
  Factories().clear();
}

std::vector<std::string> ScriptRegistry::GetScriptNames()
{
  std::vector<std::string> names;
  for (const auto& kv : Factories())
    names.push_back(kv.first);
  return names;
}

void ScriptRegistry::ListAll()
{
  for (auto& kv : Factories())
    std::cout << "Registered script: " << kv.first << "\n";
}

namespace ScriptReg
{
  script_interface* Create(flex_interface& Flex, const std::string& name)
  {
    auto it = FactoriesV2.find(name);
    if (it != FactoriesV2.end())
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

    FactoriesV2.clear();
    for (auto p = pFunc(); p != nullptr; p = p->m_pNext)
    {
      printf("Adding Script %s\n", p->m_pName);
      auto it = FactoriesV2.find(std::string(p->m_pName));
      assert(it == FactoriesV2.end() && "Duplicate script name in registration!");
      FactoriesV2[p->m_pName] = p;
    }
  }

}

void ScriptRegistry::RegisterAllScripts(HMODULE hModule)
{
  if (hModule == NULL)
  {
    printf("Error: hModule is null. Cannot register scripts.\n");
    return;
  }

  // Step 1: Get the base address of the DLL
  PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
  PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)pDosHeader + pDosHeader->e_lfanew);

  if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
  {
    printf("Error: Invalid PE signature.\n");
    return;
  }

  // Step 2: Get the address of the export directory
  PIMAGE_EXPORT_DIRECTORY pExportDir = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

  // Step 3: Get the arrays of names, ordinals, and addresses
  DWORD* pNamePtr = (DWORD*)((BYTE*)hModule + pExportDir->AddressOfNames);
  DWORD* pAddressPtr = (DWORD*)((BYTE*)hModule + pExportDir->AddressOfFunctions);
  WORD* pOrdinalPtr = (WORD*)((BYTE*)hModule + pExportDir->AddressOfNameOrdinals);

  // Step 4: Iterate through all exported functions
  for (DWORD i = 0; i < pExportDir->NumberOfNames; i++)
  {
    // Get the name of the exported function
    const char* funcName = (const char*)((BYTE*)hModule + pNamePtr[i]);

    // Check if the function name starts with our registration prefix
    std::string name(funcName);
    if (name.rfind("Register_", 0) == 0) // check if string starts with "Register_"
    {
      // Get the function's address using its ordinal
      void* funcAddr = (BYTE*)hModule + pAddressPtr[pOrdinalPtr[i]];

      // Cast to a function pointer type and call it
      using RegFunction = void(*)();
      RegFunction regFn = (RegFunction)funcAddr;
      if (regFn)
      {
        printf("Found and calling registration function: %s\n", funcName);
        regFn();
      }
    }
  }
}