#include "system.h"
#include "linux_parser.h"
#include "process.h"

#include <vector>
#include <algorithm>  // for std::sort

using std::vector;

// ---------------- CPU Utilization ----------------
float System::CpuUtilization() {
    return cpu_.Utilization();
}

// ---------------- Build or Refresh Process List ----------------
void System::UpdateProcesses() {
    processes_.clear();

    // Retrieve all PIDs
    vector<int> pids = LinuxParser::Pids();

    // Total active jiffies for normalization
    long total_active = LinuxParser::ActiveJiffies();
    if (total_active == 0) total_active = 1; // avoid div-by-zero

    for (int pid : pids) {
        Process proc(pid);

        // Safely parse data for each process
        proc.SetUser(LinuxParser::User(pid));
        proc.SetCommand(LinuxParser::Command(pid));
        proc.SetRam(LinuxParser::Ram(pid));
        proc.SetUpTime(LinuxParser::UpTime(pid));

        // CPU usage (fraction of total)
        long proc_active = LinuxParser::ActiveJiffies(pid);
        float cpu_fraction = static_cast<float>(proc_active) / static_cast<float>(total_active);
        proc.SetCpuUtilization(cpu_fraction);

        // Push valid processes only
        if (!proc.Command().empty())
            processes_.push_back(proc);
    }

    // Sort descending by CPU usage
    std::sort(processes_.begin(), processes_.end(), 
              [](Process& a, Process& b) {
                  return a.CpuUtilization() > b.CpuUtilization();
              });
}

// ---------------- Return Processes ----------------
std::vector<Process>& System::Processes() {
    // ✅ Always refresh before returning (important)
    UpdateProcesses();
    return processes_;
}

// ---------------- Memory, Uptime, etc. ----------------
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
long System::UpTime() { return LinuxParser::UpTime(); }
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
std::string System::Kernel() { return LinuxParser::Kernel(); }
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
float System::NetworkRx() { return LinuxParser::NetworkRx(); }
float System::NetworkTx() { return LinuxParser::NetworkTx(); }
