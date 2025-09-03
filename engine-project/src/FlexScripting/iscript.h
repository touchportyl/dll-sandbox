#pragma once

#include <string>

class Script
{
public:
  virtual ~Script() = default;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual std::string GetName() const = 0;
};