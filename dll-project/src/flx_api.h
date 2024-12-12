#pragma once

#ifdef _WIN32

// Remember to define FLX_BUILD_DLL in the project settings for the FlexEngine library.
#ifdef FLX_BUILD_DLL

  #define __FLX_API __declspec(dllexport)

#else

  #define __FLX_API __declspec(dllimport)

#endif

#else

  #define __FLX_API

#endif