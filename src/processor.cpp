#include "processor.h"
#include "linux_parser.h"
#include <deque>

static std::deque<float> cpu_history;

float Processor::Utilization() {
  float current = LinuxParser::ActiveJiffies() /
                  static_cast<float>(LinuxParser::Jiffies());
  cpu_history.push_back(current);
  if (cpu_history.size() > 20) cpu_history.pop_front();
  return current;
}

std::deque<float> Processor::History() { return cpu_history; }
