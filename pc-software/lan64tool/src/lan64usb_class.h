#ifndef LAN64USB_H
#define LAN64USB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libusb.h>

#include "../../firmware/usbconfig.h"  /* for device VID, PID, vendor name and product name */

class Lan64USBClass
{
public:
    Lan64USBClass();
    ~Lan64USBClass();

    bool Open();
    void Update();
    bool GetRestoreStatus();
    void SendHeader(void);
    int SendPRG(char* filename);

private:
    libusb_device* openDevice(void);
    char *usbErrorMessage(int errCode);
    bool readKeys();
    bool SendBuffer();

    libusb_device* usb_device;
    libusb_context* libusb_ctx;            // LibUSB Context (Stellt eine unabhängide LibUSB Session)

    bool isUSBOpen;
    unsigned char buffer[129];  // Puffergröße + 1 [0] enthält normalerweise die Report ID (Beim auswerten berücksichtigen)
    unsigned char prg_buffer[0x10000];
};

#endif // LAN64USB_H
