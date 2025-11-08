#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::string;
using std::vector;

namespace LinuxParser {

// --- Paths ---
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};

// --- CPU ---
vector<string> CpuUtilization() {
    string line, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    vector<string> values;
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> value; // skip "cpu"
        while (linestream >> value) {
            values.push_back(value);
        }
    }
    return values;
}

long Jiffies() { return ActiveJiffies() + IdleJiffies(); }

long ActiveJiffies() {
    vector<string> cpu = CpuUtilization();
    return stol(cpu[CPUStates::kUser_]) + stol(cpu[CPUStates::kNice_]) +
           stol(cpu[CPUStates::kSystem_]) + stol(cpu[CPUStates::kIRQ_]) +
           stol(cpu[CPUStates::kSoftIRQ_]) + stol(cpu[CPUStates::kSteal_]);
}

long IdleJiffies() {
    vector<string> cpu = CpuUtilization();
    return stol(cpu[CPUStates::kIdle_]) + stol(cpu[CPUStates::kIOwait_]);
}

long ActiveJiffies(int pid) {
    string line, value;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        vector<string> values;
        while (linestream >> value) values.push_back(value);
        if (values.size() > 17) {
            return stol(values[13]) + stol(values[14]) + stol(values[15]) + stol(values[16]);
        }
    }
    return 0;
}

// --- System ---
vector<int> Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        if (file->d_type == DT_DIR) {
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
                pids.push_back(std::stoi(filename));
            }
        }
    }
    closedir(directory);
    return pids;
}

float MemoryUtilization() {
    string line, key, value, unit;
    float total_mem = 0, free_mem = 0;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value >> unit;
            if (key == "MemTotal:") total_mem = std::stof(value);
            if (key == "MemFree:") free_mem = std::stof(value);
        }
    }
    return (total_mem - free_mem) / total_mem;
}

long UpTime() {
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        long uptime;
        linestream >> uptime;
        return uptime;
    }
    return 0;
}

std::string OperatingSystem() {
    string line, key, value;
    std::ifstream stream(kOSPath);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "PRETTY_NAME") return value;
        }
    }
    return "";
}

std::string Kernel() {
    string line, os, version, kernel;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

int TotalProcesses() {
    string line, key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "processes") return std::stoi(value);
        }
    }
    return 0;
}

int RunningProcesses() {
    string line, key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "procs_running") return std::stoi(value);
        }
    }
    return 0;
}

// --- Process ---
string Command(int pid) {
    string line;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) std::getline(stream, line);
    return line;
}

string Ram(int pid) {
    string line, key, value;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "VmRSS:") return std::to_string(std::stoi(value) / 1024);
        }
    }
    return "0";
}

string Uid(int pid) {
    string line, key, value;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "Uid:") return value;
        }
    }
    return "";
}

string User(int pid) {
    string uid = Uid(pid);
    string line, name, x, id;
    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> name >> x >> id;
            if (id == uid) return name;
        }
    }
    return "";
}

long UpTime(int pid) {
    string line, value;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        vector<string> values;
        while (linestream >> value) values.push_back(value);
        if (values.size() > 21) return stol(values[21]) / sysconf(_SC_CLK_TCK);
    }
    return 0;
}

// --- Network ---
float NetworkRx() {
    std::ifstream file(kProcDirectory + "net/dev");
    string line;
    std::getline(file, line);  // skip headers
    std::getline(file, line);
    float total_rx = 0.0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        string iface;
        long rx_bytes;
        iss >> iface >> rx_bytes;
        total_rx += rx_bytes;
    }
    return total_rx / (1024.0 * 1024.0);
}

float NetworkTx() {
    std::ifstream file(kProcDirectory + "net/dev");
    string line;
    std::getline(file, line);
    std::getline(file, line);
    float total_tx = 0.0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        string iface;
        long rx, tx;
        iss >> iface;
        for (int i = 0; i < 8; i++) iss >> rx;
        iss >> tx;
        total_tx += tx;
    }
    return total_tx / (1024.0 * 1024.0);
}

}  // namespace LinuxParser
