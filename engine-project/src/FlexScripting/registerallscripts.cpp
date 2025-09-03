#include "FlexScripting/scriptregistry.h"
#include <windows.h>
#include <cstdio>
#include <string>

// This function needs to be a part of your executable or engine,
// not the DLL with the scripts.
extern "C" __declspec(dllexport)
