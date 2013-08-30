#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <cstdio>
#include <cassert>

inline void Log(const char* msg, ...)
{
  va_list argList;
  va_start(argList, msg);
  vprintf(msg, argList);
  printf("\n");
  va_end(argList);
}

inline void Error(const char* msg, ...)
{
  va_list argList;
  va_start(argList, msg);
  vprintf(msg, argList);
  printf("\n");
  va_end(argList);
  exit(EXIT_FAILURE);
}

#endif
