#pragma once

#include "iscript.h"

// Implementation of flex_interface
// Any functions that scripts need to call to interact with the engine should be added here
class flex_interface_impl : public flex_interface
{
  void CreateEntity(void) override
  {

  }

};

inline flex_interface_impl g_flex_interface;