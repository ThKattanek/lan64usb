#ifndef LAN64USB_H
#define LAN64USB_H

#include <stdio.h>
#include "hiddata.h"
#include "firmware/usbconfig.h"  /* for device VID, PID, vendor name and product name */

class Lan64USBClass
{
public:
    Lan64USBClass();
    ~Lan64USBClass();
    bool Open();
    void Update();
    //unsigned char GetKeysRow(unsigned char col);
    //unsigned char GetKeysCol(unsigned char row);
    bool GetRestoreStatus();

private:
    usbDevice_t *dev;
    usbDevice_t *openDevice(void);
    char *usbErrorMessage(int errCode);
    bool readKeys();
    bool isUSBOpen;
    unsigned char KeyBuffer[17];  // Puffergröße + 1 [0] enthält normalerweise die Report ID (Beim auswerten berücksichtigen)
};

#endif // LAN64USB_H
