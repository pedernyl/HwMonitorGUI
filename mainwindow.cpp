#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <string>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a timer to update system information every 2 seconds
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSystemInfo);
    timer->start(2000);  // 2 seconds
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSystemInfo()
{
    // Update CPU, RAM, and Swap usage
    double cpuUsage = getCpuUsage();
    double ramUsage = getRamUsage();
    double swapUsage = getSwapUsage();

    // Update the progress bars with new values
    ui->CPUProgressBar->setValue(static_cast<int>(cpuUsage));
    ui->RAMProgressBar->setValue(static_cast<int>(ramUsage));
    ui->SwapProgressBar->setValue(static_cast<int>(swapUsage));
}

double MainWindow::getCpuUsage()
{
    std::ifstream statfile("/proc/stat");
    if (!statfile.is_open()) {
        return 0.0;
    }

    std::string line;
    std::getline(statfile, line);  // Read the first line with CPU stats
    statfile.close();

    size_t start = line.find("cpu") + 3;
    std::istringstream ss(line.substr(start));

    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idleTime = idle + iowait;
    long activeTime = user + nice + system + irq + softirq + steal;

    return (static_cast<double>(activeTime) / (activeTime + idleTime)) * 100.0;
}

double MainWindow::getRamUsage()
{
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return 0.0;
    }

    std::string line;
    long totalMem = 0, freeMem = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            totalMem = std::stol(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("MemFree:") == 0) {
            freeMem = std::stol(line.substr(line.find_first_of("0123456789")));
        }
    }
    meminfo.close();

    long usedMem = totalMem - freeMem;
    return (totalMem > 0) ? (static_cast<double>(usedMem) / totalMem) * 100.0 : 0.0;
}

double MainWindow::getSwapUsage()
{
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return 0.0;
    }

    std::string line;
    long totalSwap = 0, freeSwap = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("SwapTotal:") == 0) {
            totalSwap = std::stol(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("SwapFree:") == 0) {
            freeSwap = std::stol(line.substr(line.find_first_of("0123456789")));
        }
    }
    meminfo.close();

    long usedSwap = totalSwap - freeSwap;
    return (totalSwap > 0) ? (static_cast<double>(usedSwap) / totalSwap) * 100.0 : 0.0;
}
