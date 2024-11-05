#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void getMemoryInfo() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        std::cerr << "Could not open /proc/meminfo" << std::endl;
        return;
    }

    std::string line;
    long totalMem = 0, freeMem = 0, totalSwap = 0, freeSwap = 0;

    // Read lines from /proc/meminfo
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

    std::cout << "\033[2J\033[1;1H"; // Clear the terminal for each update
    std::cout << "RAM Usage:" << std::endl;
    std::cout << "  Total RAM: " << totalMem / 1024 << " MB" << std::endl;
    std::cout << "  Used RAM: " << usedMem / 1024 << " MB" << std::endl;
    std::cout << "  Free RAM: " << freeMem / 1024 << " MB" << std::endl;

    std::cout << "Swap Usage:" << std::endl;
    std::cout << "  Total Swap: " << totalSwap / 1024 << " MB" << std::endl;
    std::cout << "  Used Swap: " << usedSwap / 1024 << " MB" << std::endl;
    std::cout << "  Free Swap: " << freeSwap / 1024 << " MB" << std::endl;
}

int main() {
    while (true) {
        getMemoryInfo();
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Update every 2 seconds
    }
    return 0;
}

