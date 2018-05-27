//////////////////////////////////////////////////
//                                              //
// lan64tool                                    //
// von Thorsten Kattanek                        //
//                                              //
// #file: lan64usb_class.h                      //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 27.05.2018                //
// www.emu64.de                                 //
//                                              //
//////////////////////////////////////////////////

#ifndef LAN64USB_H
#define LAN64USB_H

#include <iostream>
#include <stdio.h>
#include <libusb.h>
#include <string.h>

using namespace std;

#include "../../firmware/usbconfig.h"  /* for device VID, PID, vendor name and product name */

static const int CONTROL_REQUEST_TYPE_IN = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_DEVICE;
static const int CONTROL_REQUEST_TYPE_OUT = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_DEVICE;

static const int HID_GET_REPORT = 0x01;
static const int HID_SET_REPORT = 0x09;
static const int HID_REPORT_TYPE_INPUT = 0x01;
static const int HID_REPORT_TYPE_OUTPUT = 0x02;
static const int HID_REPORT_TYPE_FEATURE = 0x03;
static const int INTERFACE_NUMBER = 0;
static const int TIMEOUT_MS = 5000;

class Lan64USBClass
{
public:
    Lan64USBClass();
    ~Lan64USBClass();

    bool Open();
    int SendPRG(char* filename);

private:
    libusb_device* openDevice(const uint16_t idVendor, const uint16_t idProduct, const char *vendorName, const char *productName, int usesReportIDs);
    void closeDevice(libusb_device_handle *device_handle);
    bool sendBuffer();

    libusb_device_handle* usb_device_handle;    // Handle des geöffneten USB Device
    libusb_device* usb_device;                  // geöffnetes USB Device
    libusb_context* libusb_ctx;                 // LibUSB Context (Stellt eine unabhängide LibUSB Session)
    libusb_device** device_list;

    bool isLibUSBSessionOpen;                   // Zeigt an ob eine LibUSB Session gestartet werden konnet
    bool isUSBDeviceOpen;                       // Zeigt an ob das USB Gerät mit Open() geöffnet werden konnte
    int usesReportIDs;
    unsigned char buffer[129];                  // Puffergröße + 1 [0] enthält normalerweise die Report ID (Beim auswerten berücksichtigen)
    unsigned char prg_buffer[0x10000];          // Puffer der das komplette c64 Programm enthält
};

#endif // LAN64USB_H
