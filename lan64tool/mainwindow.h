#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include "lan64tool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_SendPRG_clicked();

private:
    Ui::MainWindow *ui;
    Lan64USBClass *lan64;
    bool Lan64USB_Enabled;
};

#endif // MAINWINDOW_H
