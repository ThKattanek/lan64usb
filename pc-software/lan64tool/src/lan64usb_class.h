#ifndef LAN64USB_H
#define LAN64USB_H

#include <iostream>
#include <stdio.h>
#include <libusb.h>
#include <string.h>
//#include <stdlib.h>

using namespace std;

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
    libusb_device* openDevice(const uint16_t idVendor, const uint16_t idProduct, const char *vendorName, const char *productName);
    void closeDevice(libusb_device_handle *device_handle);
    char *usbErrorMessage(int errCode);
    bool readKeys();
    bool SendBuffer();

    libusb_device_handle* usb_device_handle;// Handle des geöffneten USB Device
    libusb_device* usb_device;              // geöffnetes USB Device
    libusb_context* libusb_ctx;             // LibUSB Context (Stellt eine unabhängide LibUSB Session)
    libusb_device** device_list;

    bool isLibUSBSessionOpen;               // Zeigt an ob eine LibUSB Session gestartet werden konnet
    bool isUSBDeviceOpen;                   // Zeigt an ob das USB Gerät mit Open() geöffnet werden konnte
    unsigned char buffer[129];              // Puffergröße + 1 [0] enthält normalerweise die Report ID (Beim auswerten berücksichtigen)
    unsigned char prg_buffer[0x10000];
};

#endif // LAN64USB_H
