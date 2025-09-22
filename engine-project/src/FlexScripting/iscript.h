#pragma once

#include <string>

// Interface that scripts will use to interact with the engine
struct flex_interface
{
  virtual ~flex_interface() = default;
  virtual void CreateEntity(void) = 0;
};

// Base interface for all scripts
struct script_interface
{
  virtual ~script_interface() = default;
  virtual std::string GetName() const = 0;
  virtual void Start() {}
  virtual void Stop() {}
};

// Base script implementation that scripts can derive from
struct Script : script_interface
{
  virtual ~Script() = default;
  flex_interface& m_Flex;
  Script(flex_interface& flex) : m_Flex(flex) { Awake(); }
  virtual void Awake() {}
};

