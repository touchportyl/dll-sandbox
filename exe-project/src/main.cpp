#include "entrypoint.h"

#include "intmanager.h"

class CustomApp : public FlexEngine::Application
{
  IntManager int_manager;

public:
  CustomApp()
  {
    int_manager.Init();
  }

  ~CustomApp()
  {
    int_manager.Shutdown();
  }
};

namespace FlexEngine
{
  Application* CreateApplication()
  {
    return new CustomApp();
  }
}
