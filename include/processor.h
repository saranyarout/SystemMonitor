#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <deque>

class Processor {
 public:
  float Utilization();
  std::deque<float> History();  // ✅ new
};

#endif
