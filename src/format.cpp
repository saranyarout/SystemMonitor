#include "format.h"
#include <iomanip>
#include <ncurses.h>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  long secs = seconds % 60;
  stringstream ss;
  ss << std::setfill('0') << std::setw(2) << hours << ":"
     << std::setfill('0') << std::setw(2) << minutes << ":"
     << std::setfill('0') << std::setw(2) << secs;
  return ss.str();
}

// ✅ NEW: Colored CPU usage bar
string Format::CpuBar(float percentage) {
  int size = 50;
  int bars = static_cast<int>(percentage * size);
  std::stringstream ss;
  ss << "[";
  for (int i = 0; i < size; ++i) {
    if (i < bars) {
      if (percentage > 0.75)
        attron(COLOR_PAIR(1));
      else if (percentage > 0.5)
        attron(COLOR_PAIR(2));
      else
        attron(COLOR_PAIR(3));
      ss << "|";
      attroff(COLOR_PAIR(1));
      attroff(COLOR_PAIR(2));
      attroff(COLOR_PAIR(3));
    } else {
      ss << " ";
    }
  }
  ss << "]";
  return ss.str();
}
