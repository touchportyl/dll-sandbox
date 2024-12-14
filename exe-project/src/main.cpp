#include "entrypoint.h"

#include "scriptregistry.h"
#include "intmanager.h"

#include <windows.h>

#include <iostream>

class CustomApp : public FlexEngine::Application
{
  IntManager int_manager;
  HMODULE hModule;
  typedef void (*FunctionType)();

public:
  CustomApp()
  {
    int_manager.Init();

    // run dll
    hModule = LoadLibraryA("scripting-project.dll");
    if (!hModule)
    {
      std::cout << "Failed to load DLL." << std::endl;
      return;
    }

    auto& scripts = ScriptRegistry::GetScripts();
    for (auto* script : scripts)
    {
      std::cout << "\nRunning script: " << script->GetName() << std::endl;
      script->Start();
    }

    for (auto* script : scripts)
    {
      script->Stop();
    }
  }

  ~CustomApp()
  {
    int_manager.Shutdown();
    FreeLibrary(hModule);
  }
};

namespace FlexEngine
{
  Application* CreateApplication()
  {
    return new CustomApp();
  }
}
