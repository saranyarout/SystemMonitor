#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "process.h"
#include "processor.h"

class System {
public:
    // CPU
    float CpuUtilization();

    // Memory
    float MemoryUtilization();

    // Processes
    std::vector<Process>& Processes();

    // Uptime
    long UpTime();

    // Total processes
    int TotalProcesses();

    // Running processes
    int RunningProcesses();

    // Kernel version
    std::string Kernel();

    // OS name
    std::string OperatingSystem();

    // Refresh process list
    void UpdateProcesses();

    // Network usage
    float NetworkRx();
    float NetworkTx();

private:
    Processor cpu_;
    std::vector<Process> processes_;
};

#endif
