#include "process.h"
#include "linux_parser.h"

#include <string>

using std::string;

// Constructor that sets pid
Process::Process(int pid) : pid_(pid) {}

// ---------- Getters ----------
int Process::Pid() const { return pid_; }
string Process::User() const { return user_; }
string Process::Command() const { return command_; }
float Process::CpuUtilization() const { return cpu_utilization_; }
string Process::Ram() const { return ram_; }
long Process::UpTime() const { return up_time_; }

// ---------- Setters ----------
void Process::SetPid(int pid) { pid_ = pid; }
void Process::SetUser(const string& user) { user_ = user; }
void Process::SetCommand(const string& command) { command_ = command; }
void Process::SetCpuUtilization(float cpu) { cpu_utilization_ = cpu; }
void Process::SetRam(const string& ram) { ram_ = ram; }
void Process::SetUpTime(long up_time) { up_time_ = up_time; }

// ---------- Operators ----------
bool Process::operator<(const Process& a) const {
  return cpu_utilization_ < a.cpu_utilization_;
}

bool Process::operator>(const Process& a) const {
  return cpu_utilization_ > a.cpu_utilization_;
}
