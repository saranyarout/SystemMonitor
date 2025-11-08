#ifndef LINUX_PARSER_H
#define LINUX_PARSER_H

#include <string>
#include <vector>

namespace LinuxParser {

// --- Paths ---
extern const std::string kProcDirectory;
extern const std::string kCmdlineFilename;
extern const std::string kStatusFilename;
extern const std::string kStatFilename;
extern const std::string kUptimeFilename;
extern const std::string kMeminfoFilename;
extern const std::string kVersionFilename;
extern const std::string kOSPath;
extern const std::string kPasswordPath;

// CPU States
enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
};

// --- Functions ---

// CPU
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// System
std::string OperatingSystem();
std::string Kernel();
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();

// Process
std::string Command(int pid);
std::string Ram(int pid);
std::string User(int pid);
std::string Uid(int pid);
long UpTime(int pid);
long ActiveJiffies(int pid);

// Network
float NetworkRx();
float NetworkTx();

}  // namespace LinuxParser

#endif
