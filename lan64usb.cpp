#include "lan64usb.h"

Lan64USBClass::Lan64USBClass()
{
    dev = NULL;
    isUSBOpen = false;
}

Lan64USBClass::~Lan64USBClass()
{
    usbhidCloseDevice(dev);
}

usbDevice_t *Lan64USBClass::openDevice(void)
{
usbDevice_t     *dev = NULL;
unsigned char   rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char            vendorName[] = {USB_CFG_VENDOR_NAME, 0}, productName[] = {USB_CFG_DEVICE_NAME, 0};
int             vid = rawVid[0] + 256 * rawVid[1];
int             pid = rawPid[0] + 256 * rawPid[1];
int             err;

    if((err = usbhidOpenDevice(&dev, vid, vendorName, pid, productName, 0)) != 0)
    {
        fprintf(stderr, "error finding %s: %s\n", productName, usbErrorMessage(err));
        return NULL;
    }
    return dev;
}

char *Lan64USBClass::usbErrorMessage(int errCode)
{
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
    return NULL;    /* not reached */
}

bool Lan64USBClass::Open()
{
    dev = openDevice();
    if(dev == NULL)
    {
        isUSBOpen = false;
    }
    else
    {
        isUSBOpen = true;
    }

    return isUSBOpen;
}

bool Lan64USBClass::SendBuffer()
{
    int err;

    buffer[0] = 0;
    buffer[1] = 0;

    if((err = usbhidSetReport(dev,(char*)buffer, sizeof(buffer))) != 0)
    {
        fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        return false;
    }
    else
    {
        return false;
    }
}

void Lan64USBClass::SendHeader()
{
    unsigned short Startadresse = 0xC000;
    unsigned short Size = 120;

    buffer[0+2]=0;
    buffer[1+2]=(unsigned char)Startadresse;
    buffer[2+2]=Startadresse>>8;
    buffer[3+2]=(unsigned char)Size;
    buffer[4+2]=Size>>8;
    buffer[6+2]=Startadresse+Size;
    buffer[7+2]=(Startadresse+Size)>>8;

    unsigned char temp=Size/256;
    if((Size-(temp*256))>0) temp++;

    buffer[5+2]=temp;

    /// Hader senden
    SendBuffer();
    for(int i=0;i<128;i++) buffer[i+2] = 0;
    SendBuffer();


    for(int i=0;i<128;i++) buffer[i+2] = i;
    SendBuffer();
    for(int i=0;i<128;i++) buffer[i+2] = i+128;
    SendBuffer();
}

