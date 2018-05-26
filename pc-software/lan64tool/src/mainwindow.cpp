#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // USB Gerät öffnen
    lan64 = new Lan64USBClass();
    Lan64USB_Enabled = lan64->Open();

    if(!Lan64USB_Enabled)
    {
        QMessageBox::critical(this,trUtf8("Lan64USB Adapter ..."),trUtf8("Es konnte kein Lan64USB Adapter gefunden werden.\nBitte überprüfen Sie ob das Gerät eingesteckt ist,\noder ob Sie über genügend Rechte verfügen."));
        exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SendPRG_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,trUtf8("C64 Datei öffnen"),"",trUtf8("C64 Programm Dateien (*.prg *.c64);;Alle Dateien(*.*)"));
    if(filename != "")
    {
        if(!lan64->SendPRG(filename.toLatin1().data()))
        {
            //QMessageBox::warning(this,"Error...","Datei konnte nicht übertragen werden.");
        }
    }
}
