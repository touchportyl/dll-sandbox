#pragma once

#include "flx_api.h"
#include "iscript.h"

#include <vector>

// A global registry to store all scripts
class __FLX_API ScriptRegistry
{
public:
  static void RegisterScript(Script* script);
  static std::vector<Script*>& GetScripts();
};