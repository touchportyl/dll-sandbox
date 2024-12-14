#include "application.h"

#include <iostream>

namespace FlexEngine
{

  Application::Application()
  {
  }

  Application::~Application()
  {
  }

  void Application::Run()
  {
    std::cout << "Application is running..." << std::endl;
  }

}