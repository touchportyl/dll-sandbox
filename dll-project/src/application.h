#pragma once

#include "flx_api.h"

int main(int, char**);

namespace FlexEngine
{

  // The architecture of the engine is designed to only have one application instance.
  class __FLX_API Application
  {
    static bool m_is_closing;

  public:
    Application();
    virtual ~Application();

  private:

    static void Run();

    friend int ::main(int, char**);
  };

  Application* CreateApplication();
}
