#include "scriptregistry.h"

void ScriptRegistry::RegisterScript(Script * script)
{
  GetScripts().push_back(script);
}

std::vector<Script*>& ScriptRegistry::GetScripts()
{
  static std::vector<Script*> scripts;
  return scripts;
}
