#include <iostream>
#include <string.h>

using namespace std;

void help_out(void);

int main(int argc, char *argv[])
{
    /// Comandline auswerten ///

    bool error = false;

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


    // Datei Übertragen
}

void help_out(void)
{
    cout << endl << "usage: lan64_send 'c64 file'" << endl;
    cout << "Tranfer a C64 File to C64 over the LAN64 USB" << endl << endl;
    cout << "--help\t\tThis shows the help" << endl;
    cout << "--version\tThis shows the version information" << endl;
}
