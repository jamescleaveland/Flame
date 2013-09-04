#include <chrono>
#include "Util.h"

#define PERF_START_TIMER(str) \
  auto perfStartTime = std::chrono::steady_clock::now(); \
  Log("%s", str);

#define PERF_END_TIMER(str) \
  auto perfEndTime = std::chrono::steady_clock::now(); \
  auto perfDelta = perfEndTime - perfStartTime; \
  Log("%s %3.5f sec", str, std::chrono::duration<double, std::milli>(perfDelta).count() * 0.001);
