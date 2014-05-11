#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->wert_out->setText("0");

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

void MainWindow::on_scroll_wert_valueChanged(int value)
{
    ui->wert_out->setText(QVariant(value).toString());
}

void MainWindow::on_pushButton_clicked()
{
    lan64->SendHeader();
}
