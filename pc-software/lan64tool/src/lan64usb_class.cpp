#include "lan64usb_class.h"

Lan64USBClass::Lan64USBClass()
{
    usb_device = NULL;
    isUSBDeviceOpen = false;

    // LibUSB Session beginnen
    int ret = libusb_init(&libusb_ctx);

    if(ret < 0)
    {
        isLibUSBSessionOpen = false;
        cerr << "Fehler beim öffnen eine LibUSB-1 Session!" << endl;
    }
    else
        isLibUSBSessionOpen = true;
}

Lan64USBClass::~Lan64USBClass()
{
    // geöffnetes Device schließen
    closeDevice(usb_device_handle);

    // LibUSB Session beenden
    libusb_exit(libusb_ctx);
}

libusb_device* Lan64USBClass::openDevice(const uint16_t idVendor,const uint16_t idProduct, const char* vendorName, const char* productName)
{
    libusb_device* usb_device;
    libusb_device_descriptor desc;

    // USB Device anhand der Vendor und Protuct ID öffnen
    usb_device_handle = libusb_open_device_with_vid_pid(libusb_ctx, idVendor, idProduct);

    if(usb_device_handle == NULL)
    {
        cerr << "Es wurden kein USB Devices gefunden mit idVendor: 0x" << hex << idVendor << " und idProduct: 0x" << hex << idProduct << endl;
        return NULL;
    }

    // Device mit hilfe des Handles holen
    usb_device = libusb_get_device(usb_device_handle);

    int ret = libusb_get_device_descriptor(usb_device, &desc);
    if (ret < 0)
    {
        cerr << "Fehler beim holen des Device Descriptors." << endl;
        return NULL;
    }

    char string[256];
    int len = libusb_get_string_descriptor_ascii(usb_device_handle, desc.iProduct, (unsigned char*)string, sizeof(string));
    if(len < 0)
    {
        cerr << "Fehler beim auslesen des Product Namen." << endl;
        return NULL;
    }
    else
    {
        // Prüfen ob ProductName übereinstimmt
        if(strcmp(string, productName) == 0)
        {
            len = libusb_get_string_descriptor_ascii(usb_device_handle, desc.iManufacturer, (unsigned char*)string, sizeof(string));
            if(len < 0)
            {
                cerr << "Fehler beim auslesen des Vendor Namen." << endl;
                return NULL;
            }
            else
            {
                if(strcmp(string, vendorName) != 0)
                {
                    cerr << "Vendor Name stimmt nicht! SOLL: " << vendorName << " IST: " << string << endl;
                    return NULL;
                }
            }
        }
        else
        {
            cerr << "Product Name stimmt nicht! SOLL: " << productName << " IST: " << string << endl;
            return NULL;
        }
    }

    return usb_device;
}

void Lan64USBClass::closeDevice(libusb_device_handle *device_handle)
{
    libusb_close(device_handle);
}

char *Lan64USBClass::usbErrorMessage(int errCode)
{
    /*
static char buffer[80];

    switch(errCode)
    {
        case USBOPEN_ERR_ACCESS:      return (char*)"Access to device denied";
        case USBOPEN_ERR_NOTFOUND:    return (char*)"The specified device was not found";
        case USBOPEN_ERR_IO:          return (char*)"Communication error with device";
        default:
            sprintf(buffer, "Unknown USB error %d", errCode);
            return buffer;
    }
    */
    return NULL;    /* not reached */
}

bool Lan64USBClass::Open()
{
    if(!isLibUSBSessionOpen)
        return false;

    // Daten aus usbconfig.h der Firmware aufarbeiten
    unsigned char   rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char            vendorName[] = {USB_CFG_VENDOR_NAME, 0}, productName[] = {USB_CFG_DEVICE_NAME, 0};
    int             vendor_id = rawVid[0] + 256 * rawVid[1];
    int             product_id = rawPid[0] + 256 * rawPid[1];

    usb_device = openDevice(vendor_id, product_id, vendorName, productName);
    if(usb_device == NULL)
    {
        isUSBDeviceOpen = false;
    }
    else
    {
        isUSBDeviceOpen = true;
    }

    return isUSBDeviceOpen;
}

bool Lan64USBClass::SendBuffer()
{
    /*
    int err;

    buffer[0] = 0;

    if((err = usbhidSetReport(dev,(char*)buffer, sizeof(buffer))) != 0)
    {
        fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        return false;
    }
    else
    {
        return false;
    }
    */
}

int Lan64USBClass::SendPRG(char* filename)
{
    unsigned short StartAdresse;
    unsigned short Size;

    unsigned char temp[2];
    FILE* file = fopen (filename, "rb");
    if (file == NULL)
    {
        return 0x01;
    }

    Size = fread (&temp,1,2,file);
    StartAdresse=temp[0]|(temp[1]<<8);

    Size = fread (prg_buffer,1,0x10000,file);

    fclose(file);

    /// Header vorbereiten
    for(int i=0;i<128;i++) buffer[i+1] = 0;

    buffer[0+1]=0;
    buffer[1+1]=(unsigned char)StartAdresse;
    buffer[2+1]=StartAdresse>>8;
    buffer[3+1]=(unsigned char)Size;
    buffer[4+1]=Size>>8;
    buffer[5+1]=Size>>8;
    if((Size-(buffer[5+1]<<8))>0) buffer[5+1]++;
    buffer[6+1]=StartAdresse+Size;
    buffer[7+1]=(StartAdresse+Size)>>8;

    /// Hader senden
    SendBuffer();   // 1.128Byte
    SendBuffer();   // 2.128Byte


    /// Programm übertragen

    int Blockanzahl = buffer[5+1]<<1;
    int ProgZeiger = 0;

    for(int i=0; i<Blockanzahl; i++)
    {
        for(int ii=0;ii<128;ii++)
            buffer[ii+1] = prg_buffer[ProgZeiger + ii];
        SendBuffer();
        ProgZeiger += 128;
    }


    return 0;
}

void Lan64USBClass::SendHeader()
{
    unsigned short StartAdresse = 0x0800;
    unsigned short Size = 51199;

    for(int i=0;i<128;i++) buffer[i+1] = 0;

    buffer[0+1]=0;
    buffer[1+1]=(unsigned char)StartAdresse;
    buffer[2+1]=StartAdresse>>8;
    buffer[3+1]=(unsigned char)Size;
    buffer[4+1]=Size>>8;
    buffer[5+1]=Size>>8;
    if((Size-(buffer[5+1]<<8))>0) buffer[5+1]++;
    buffer[6+1]=StartAdresse+Size;
    buffer[7+1]=(StartAdresse+Size)>>8;

    /// Hader senden
    SendBuffer();   // 1.128Byte
    SendBuffer();   // 2.128Byte

    for(int i=0;i<128;i++) buffer[i+1] = i;

    for(int i=0; i<200; i++)
    {
        SendBuffer();
        SendBuffer();
    }
}

