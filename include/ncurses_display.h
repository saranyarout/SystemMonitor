#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>
#include <string>
#include <vector>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {

// Display the full system monitor
void Display(System& system, int n = 10);

// Display system metrics (CPU, Memory, Uptime, Network)
void DisplaySystem(System& system, WINDOW* window);

// Display top N processes
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);

// Display horizontal progress bar for CPU, Memory, Network
std::string ProgressBar(float percent);

// Display network statistics (RX/TX)
void DisplayNetwork(System& system, WINDOW* window);

};  // namespace NCursesDisplay

#endif
