#include <iostream>
#include <string.h>

#include "../../lan64tool/lan64usb_class.h"

using namespace std;

void help_out(void);

int main(int argc, char *argv[])
{
    /// Comandline auswerten ///

    Lan64USBClass *lan64;
    bool Lan64USB_Enabled = false;

    if(argc > 1)
    {
        if(strcmp(argv[1],"--help") == 0)
        {
            help_out();
            return (0);
        }

        if(strcmp(argv[1],"--version") == 0)
        {
            cout << endl << "Version 0.10 [Code by Thorsten Kattanek (C)2014]" << endl;
            return (0);
        }
    }

    if(argc > 2 || argc == 1)
    {
        help_out();
        return (0);
    }

    // Lan64 USB öffnen
    lan64 = new Lan64USBClass();
    Lan64USB_Enabled = lan64->Open();

    // Datei Übertragen
    if(!Lan64USB_Enabled)
    {
        cout << endl << "Es konnte kein Lan64USB Adapter gefunden werden.\nBitte überprüfen Sie ob das Gerät eingesteckt ist,\noder ob Sie über genügend Rechte verfügen." << endl;
        return (0);
    }

    if(0x01 == lan64->SendPRG(argv[1]))
    {
        cout << endl << "Die Datei konnte nicht geoeffnet werden." << endl;
    }

    delete lan64;
}

void help_out(void)
{
    cout << endl << "usage: lan64_send 'c64 file'" << endl;
    cout << "Tranfer a C64 File to C64 over the LAN64 USB" << endl << endl;
    cout << "--help\t\tThis shows the help" << endl;
    cout << "--version\tThis shows the version information" << endl;
}
