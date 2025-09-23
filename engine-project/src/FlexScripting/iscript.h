#pragma once

#include <string>

// Interface that scripts will use to interact with the engine
struct EngineInterface
{
  virtual ~EngineInterface() = default;
  virtual void CreateEntity(void) = 0;
};

// Base interface for all scripts
struct IScriptBase
{
  virtual ~IScriptBase() = default;
  virtual std::string GetName() const = 0;
  virtual void Start() {}
  virtual void Stop() {}
};

// Base script implementation that scripts can derive from
struct Script : IScriptBase
{
  virtual ~Script() = default;
  EngineInterface& m_EngineInterface;
  Script(EngineInterface& engine) : m_EngineInterface(engine) { Awake(); }
  virtual void Awake() {}
};

