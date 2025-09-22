#pragma once

#include <string>


struct flex_interface
{
  virtual ~flex_interface() = default;
  virtual void CreateEntity(void) = 0;
};

struct script_interface
{
  virtual ~script_interface() = default;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual std::string GetName() const = 0;
};

struct Script : script_interface
{
  Script(flex_interface& flex) : m_Flex(flex) {}
  flex_interface& m_Flex;
};

