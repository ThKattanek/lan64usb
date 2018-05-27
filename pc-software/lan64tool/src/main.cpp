//////////////////////////////////////////////////
//                                              //
// lan64tool                                    //
// von Thorsten Kattanek                        //
//                                              //
// #file: main.cpp                              //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 27.05.2018                //
// www.emu64.de                                 //
//                                              //
//////////////////////////////////////////////////

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
