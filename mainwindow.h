#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "lan64usb.h"

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
    void on_scroll_wert_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Lan64USBClass *lan64;
    bool Lan64USB_Enabled;
};

#endif // MAINWINDOW_H
