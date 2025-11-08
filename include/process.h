#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  // Constructors
  Process() = default;
  explicit Process(int pid);

  // Getters
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long UpTime() const;

  // Setters
  void SetPid(int pid);
  void SetUser(const std::string& user);
  void SetCommand(const std::string& command);
  void SetCpuUtilization(float cpu);
  void SetRam(const std::string& ram);
  void SetUpTime(long up_time);

  // Operators for sorting
  bool operator<(const Process& a) const;
  bool operator>(const Process& a) const;

 private:
  int pid_{0};
  std::string user_{""};
  std::string command_{""};
  float cpu_utilization_{0.0};
  std::string ram_{""};
  long up_time_{0};
};

#endif // PROCESS_H
