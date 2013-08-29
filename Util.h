#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <cstdio>
#include <cassert>

inline void Log(const char* msg)
{
  std::cout << msg << std::endl;
}

inline void Error(const char* msg, const int code = EXIT_FAILURE)
{
  Log(msg);
  exit(code);
}

#endif
