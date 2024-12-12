#include "intmanager.h"

// static member initialization
bool IntManager::is_init = false;
int* IntManager::active_object = nullptr;

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
