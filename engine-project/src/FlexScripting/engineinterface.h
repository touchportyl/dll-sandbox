#pragma once

#include "iscript.h"

// Implementation of EngineInterface
// Any functions that scripts need to call to interact with the engine should be added here
class EngineInterfaceImplementation : public EngineInterface
{
  void CreateEntity(void) override
  {

  }

};

inline EngineInterfaceImplementation g_engine_interface_implementation;