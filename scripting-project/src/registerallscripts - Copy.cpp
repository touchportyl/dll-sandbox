#include "FlexScripting/scriptregistry.h"
#include <windows.h>
#include <cstdio>
#include <cstring>

extern "C" IMAGE_DOS_HEADER __ImageBase;

extern "C" __declspec(dllexport)
void RegisterAllScripts()
{
  HMODULE hMod = (HMODULE)&__ImageBase;
  auto dos = (PIMAGE_DOS_HEADER)hMod;
  auto nt = (PIMAGE_NT_HEADERS)((BYTE*)dos + dos->e_lfanew);

  auto sec = IMAGE_FIRST_SECTION(nt);
  for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; i++, sec++)
  {
    printf("Section: %.8s\n", sec->Name);

    if (strncmp((char*)sec->Name, ".scriptr", 9) == 0)
    {
      auto start = (ScriptEntry*)((BYTE*)hMod + sec->VirtualAddress);
      auto end = (ScriptEntry*)((BYTE*)start + sec->Misc.VirtualSize);

      for (auto p = start; p < end; p++)
      {
        if (p->name && p->create)
        {
          // Call the forceLink function before registering
          if (p->forceLink)
          {
            p->forceLink();
          }
          printf("Registering script: %s\n", p->name);
          ScriptRegistry::RegisterFactory(p->name, p->create);
        }
      }
    }
  }
}