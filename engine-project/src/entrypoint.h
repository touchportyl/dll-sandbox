#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "application.h"

extern FlexEngine::Application* FlexEngine::CreateApplication();

int main(int, char**)
{
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  FlexEngine::Application* app = FlexEngine::CreateApplication();
  app->Run();
  delete app;

  return 0;
}