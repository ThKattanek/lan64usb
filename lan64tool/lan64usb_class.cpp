#include "lan64usb_class.h"

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

