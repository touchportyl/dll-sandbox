#include "intmanager.h"

// static member initialization
bool IntManager::is_init = false;
int* IntManager::active_object = nullptr;
std::unordered_map<std::string, int*> IntManager::registry;

IntManager::~IntManager()
{
  Shutdown();
}

void IntManager::Init()
{
  if (is_init) return;
  is_init = true;
  active_object = nullptr;
}

void IntManager::Shutdown()
{
  if (!is_init) return;
  is_init = false;
  active_object = nullptr;
}

bool IntManager::IsInit() const
{
  return is_init;
}

int* IntManager::GetActiveObject()
{
  if (!is_init) return nullptr;
  return active_object;
}

void IntManager::SetActiveObject(int* object)
{
  if (!is_init) return;
  active_object = object;
}

int* IntManager::CreateObject(const std::string& name)
{
  if (!is_init) return nullptr;
  if (registry.find(name) != registry.end()) return nullptr;
  int* object = new int;
  registry[name] = object;
  return object;
}

void IntManager::DestroyObject(const std::string& name)
{
  if (!is_init) return;
  auto it = registry.find(name);
  if (it == registry.end()) return;
  delete it->second;
  registry.erase(it);
}

void IntManager::DestroyObject(int* object)
{
  if (!is_init) return;
  for (auto it = registry.begin(); it != registry.end(); ++it)
  {
    if (it->second == object)
    {
      delete it->second;
      registry.erase(it);
      return;
    }
  }
}

int* IntManager::GetObject(const std::string& name)
{
  if (!is_init) return nullptr;
  auto it = registry.find(name);
  if (it == registry.end()) return nullptr;
  return it->second;
}

void IntManager::ChangeValue(int value)
{
  if (!is_init) return;
  if (active_object == nullptr) return;
  *active_object = value;
}
