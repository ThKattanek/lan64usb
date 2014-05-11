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
