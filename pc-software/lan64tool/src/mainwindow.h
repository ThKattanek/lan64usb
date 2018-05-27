//////////////////////////////////////////////////
//                                              //
// lan64tool                                    //
// von Thorsten Kattanek                        //
//                                              //
// #file: mainwindow.h                          //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 27.05.2018                //
// www.emu64.de                                 //
//                                              //
//////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include "lan64usb_class.h"

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
