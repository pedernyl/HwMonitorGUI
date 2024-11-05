#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

void getMemoryInfo() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        std::cerr << "Could not open /proc/meminfo" << std::endl;
        return;
    }

    std::string line;
    long totalMem = 0, freeMem = 0, totalSwap = 0, freeSwap = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            totalMem = std::stol(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("MemFree:") == 0) {
            freeMem = std::stol(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("SwapTotal:") == 0) {
            totalSwap = std::stol(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("SwapFree:") == 0) {
            freeSwap = std::stol(line.substr(line.find_first_of("0123456789")));
        }
    }
    meminfo.close();

    long usedMem = totalMem - freeMem;
    long usedSwap = totalSwap - freeSwap;

    std::cout << "RAM Usage:" << std::endl;
    std::cout << "  Total RAM: " << totalMem / 1024 << " MB" << std::endl;
    std::cout << "  Used RAM: " << usedMem / 1024 << " MB" << std::endl;
    std::cout << "  Free RAM: " << freeMem / 1024 << " MB" << std::endl;

    std::cout << "Swap Usage:" << std::endl;
    std::cout << "  Total Swap: " << totalSwap / 1024 << " MB" << std::endl;
    std::cout << "  Used Swap: " << usedSwap / 1024 << " MB" << std::endl;
    std::cout << "  Free Swap: " << freeSwap / 1024 << " MB" << std::endl;
}

double getCpuUsage() {
    std::ifstream statfile("/proc/stat");
    if (!statfile.is_open()) {
        std::cerr << "Could not open /proc/stat" << std::endl;
        return -1.0;
    }

    std::string line;
    std::getline(statfile, line);  // Read the first line which contains CPU stats
    statfile.close();

    size_t start = line.find("cpu") + 3;  // Skip "cpu"
    std::istringstream ss(line.substr(start));

    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idleTime = idle + iowait;
    long activeTime = user + nice + system + irq + softirq + steal;

    return static_cast<double>(activeTime) / (activeTime + idleTime) * 100.0;
}

int main() {
    while (true) {
        std::cout << "\033[2J\033[1;1H";  // Clear the terminal

        getMemoryInfo();
        
        double cpuUsage = getCpuUsage();
        if (cpuUsage >= 0) {
            std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
        } else {
            std::cerr << "Failed to retrieve CPU usage." << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Update every 1 seconds
    }
    return 0;
}
