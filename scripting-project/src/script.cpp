// Do not modify the code in this file.

#include "FlexScripting/scriptregistry.h"

extern "C"
{
  __declspec(dllexport) void* get_scripts() { return register_base::m_pHead; }
}