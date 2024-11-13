#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSystemInfo();  // Slot to update system information periodically

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    double getCpuUsage();
    double getRamUsage();
    double getSwapUsage();
};

#endif // MAINWINDOW_H
