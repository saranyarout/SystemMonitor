#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);
std::string CpuBar(float percentage);  // ✅ new
};

#endif
