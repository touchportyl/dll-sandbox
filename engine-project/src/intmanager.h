#include "flx_api.h"

#include <string>
#include <unordered_map>

#pragma once

class __FLX_API IntManager
{
private:
  static bool is_init;
public:
  ~IntManager();
  void Init();
  void Shutdown();
  bool IsInit() const;

private:
  static int* active_object;
public:
  static int* GetActiveObject();
  static void SetActiveObject(int* object);

private:
  #pragma warning(suppress: 4251) // disable warning about using STL in DLL
  static std::unordered_map<std::string, int*> registry;
public:
  static int* CreateObject(const std::string& name);
  static void DestroyObject(const std::string& name);
  static void DestroyObject(int* object);
  static int* GetObject(const std::string& name);

public:
  static void ChangeValue(int value);
};
