# 🖥️ System Monitor Tool

**Author:** Saranya Rout  
**Course:** B.Tech (CSIT)  
**Project Type:** Capstone Project  

---

## 📘 Project Title
**System Monitor Tool**

A C++ console-based system monitor that displays **real-time information** about system performance — including CPU usage, memory utilization, running processes, OS, kernel version, and uptime.  
The UI is built using **ncurses** for a dynamic, color-coded terminal display.

---

## 🌟 Features
- Displays OS and Kernel information  
- Shows CPU and Memory utilization in real time  
- Lists top processes with CPU and RAM usage  
- Updates live using ncurses  
- Color-coded output for better visualization  

---

## 🧰 Technologies Used
- **Language:** C++  
- **Library:** ncurses  
- **Build System:** CMake  
- **Platform:** Linux / Ubuntu  

---

## 📂 Repository Link
🔗 [Click here to view the full source code on GitHub](https://github.com/saranyarout/SystemMonitor)

---

## 🖼️ Output Screenshot
Below is an example output from the System Monitor tool:

![System Monitor Output](screenshots/screenshot.png)

---

## ⚙️ How to Run
```bash
# Clone the repository
git clone https://github.com/saranyarout/SystemMonitor.git

# Navigate to project folder
cd SystemMonitor/System-monitor-tool

# Build and run
rm -rf build
mkdir build && cd build
cmake ..
make
./SystemMonitor
