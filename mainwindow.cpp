#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // USB Ger�t �ffnen
    lan64 = new Lan64USBClass();
    Lan64USB_Enabled = lan64->Open();

    if(!Lan64USB_Enabled)
    {
        QMessageBox::critical(this,"Lan64USB Adapter ...","Es konnte kein Lan64USB Adapter gefunden werden.\nBitte �berpr�fen Sie ob das Ger�t eingesteckt ist,\noder ob Sie �ber gen�gend Rechte verf�gen.");
        exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SendPRG_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("C64 Dateie �ffnen "),"",tr("C64 Programm Dateien") + "(*.prg);" + "(*.c64);;" + tr("Alle Dateien") + "(*.*)");
    if(filename != "")
    {
        if(!lan64->SendPRG(filename.toLatin1().data()))
        {
            //QMessageBox::warning(this,"Error...","Datei konnte nicht �bertragen werden.");
        }
    }
}
