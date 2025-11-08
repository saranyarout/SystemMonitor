#include <curses.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <algorithm>  // ✅ for std::sort

#include "ncurses_display.h"
#include "system.h"
#include "format.h"

// Function definitions inside NCursesDisplay namespace
namespace NCursesDisplay {

void Display(System& system,int n) {
    initscr();              // Start ncurses mode
    noecho();               // Don’t echo input
    cbreak();               // Disable line buffering
    curs_set(0);            // Hide cursor
    start_color();          // Enable color
    use_default_colors();

    // Define color pairs
    init_pair(1, COLOR_CYAN, -1);     // Labels
    init_pair(2, COLOR_GREEN, -1);    // Values
    init_pair(3, COLOR_YELLOW, -1);   // CPU & Memory usage
    init_pair(4, COLOR_MAGENTA, -1);  // Titles and borders
    init_pair(5, COLOR_RED, -1);      // Critical or root

    int x_max{getmaxx(stdscr)};
    int y_max{getmaxy(stdscr)};

    // Divide screen into two sections
    WINDOW* system_window = newwin(10, x_max - 2, 0, 1);
    WINDOW* process_window = newwin(y_max - 11, x_max - 2, 11, 1);

    while (1) {
        box(system_window, 0, 0);
        box(process_window, 0, 0);

        attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(0, (x_max / 2) - 8, "=== SYSTEM MONITOR ===");
        attroff(COLOR_PAIR(4) | A_BOLD);

        DisplaySystem(system, system_window);
        DisplayProcesses(system.Processes(), process_window, 10);

        wrefresh(system_window);
        wrefresh(process_window);
        refresh();

        sleep(1);
    }

    endwin();
}

// ---------------- Display System Info ----------------
void DisplaySystem(System& system, WINDOW* window) {
    werase(window);
    box(window, 0, 0);

    int row = 1;

    // OS
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "OS: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(2));
    wprintw(window, "%s", system.OperatingSystem().c_str());
    wattroff(window, COLOR_PAIR(2));

    // Kernel
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "Kernel: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(2));
    wprintw(window, "%s", system.Kernel().c_str());
    wattroff(window, COLOR_PAIR(2));

    // CPU Utilization (updated for your System class)
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "CPU Usage: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(3) | A_BOLD);
    wprintw(window, "%.2f%%", system.CpuUtilization() * 100);
    wattroff(window, COLOR_PAIR(3) | A_BOLD);

    // Memory Utilization
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "Memory Usage: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(3) | A_BOLD);
    wprintw(window, "%.2f%%", system.MemoryUtilization() * 100);
    wattroff(window, COLOR_PAIR(3) | A_BOLD);

    // Total Processes
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "Total Processes: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(2));
    wprintw(window, "%d", system.TotalProcesses());
    wattroff(window, COLOR_PAIR(2));

    // Running Processes
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "Running Processes: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(2));
    wprintw(window, "%d", system.RunningProcesses());
    wattroff(window, COLOR_PAIR(2));

    // Uptime
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row++, 2, "Uptime: ");
    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(2));
    wprintw(window, "%s", Format::ElapsedTime(system.UpTime()).c_str());
    wattroff(window, COLOR_PAIR(2));

    wrefresh(window);
}

// ---------------- Display Process Table ----------------
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n) {
    werase(window);
    box(window, 0, 0);

    // Table header
    wattron(window, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(window, 1, 2, "%-6s %-10s %-6s %-6s %-10s %s",
              "PID", "USER", "CPU%", "RAM", "UPTIME", "COMMAND");
    wattroff(window, COLOR_PAIR(4) | A_BOLD);

    int row = 2;
    int max_rows = n;

    std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.CpuUtilization() > b.CpuUtilization();
    });

    for (int i = 0; i < std::min((int)processes.size(), max_rows); i++) {
        Process& p = processes[i];
        std::string user = p.User();
        std::string cmd = p.Command().substr(0, 30);  // Limit length

        if (user == "root")
            wattron(window, COLOR_PAIR(5));  // red for root
        else
            wattron(window, COLOR_PAIR(2));  // green for normal users

        mvwprintw(window, row++, 2, "%-6d %-10s %-6.2f %-6s %-10s %s",
                  p.Pid(), user.c_str(), p.CpuUtilization() * 100,
                  p.Ram().c_str(),
                  Format::ElapsedTime(p.UpTime()).c_str(),
                  cmd.c_str());

        wattroff(window, COLOR_PAIR(5));
        wattroff(window, COLOR_PAIR(2));
    }

    wrefresh(window);
}

}  // namespace NCursesDisplay
