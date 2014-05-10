/* Name: main.c
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 30.07.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 1.1
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 *
 * Hinweis ! Diese Firmware ist nur für die Platine mit Bedrahteten Bauteilen. V1.1 !
 *
 */

/* Pinbelegung Atmega8 in PDIP
 * PIN 02 - PD0 = LED        A
 * PIN 28 - PC5 = RESTORE    E
 * PIN 11 - PD5 = Matrix Col 0 A
 * PIN 14 - PB0 = Matrix Col 1 A
 * PIN 16 - PB2 = Matrix Col 2 A
 * PIN 13 - PD7 = Matrix Col 3 A
 * PIN 17 - PB3 = Matrix Col 4 A
 * PIN 12 - PD6 = Matrix Col 5 A
 * PIN 18 - PB4 = Matrix Col 6 A
 * PIN 15 - PB1 = Matrix Col 7 A
 * PIN 19 - PB5 = Matrix Row 0 E
 * PIN 23 - PC0 = Matrix Row 1 E
 * PIN 06 - PD4 = Matrix Row 2 E
 * PIN 27 - PC4 = Matrix Row 3 E
 * PIN 25 - PC2 = Matrix Row 4 E
 * PIN 26 - PC3 = Matrix Row 5 E
 * PIN 03 - PD1 = Matrix Row 6 E
 * PIN 24 - PC1 = Matrix Row 7 E
*/

//////////////////////////////////////
/// Je nach Hardware konfigurieren ///
//////////////////////////////////////

#define LED_N    D   //Port
#define LED_P    0   //PinNummer

#define RESTORE_N    C   //Port
#define RESTORE_P    5   //PinNummer

#define PA0N    D   // Port
#define PA0P    5   // PinNummer
#define PA1N    B   // Port
#define PA1P    0   // PinNummer
#define PA2N    B   // Port
#define PA2P    2   // PinNummer
#define PA3N    D   // Port
#define PA3P    7   // PinNummer
#define PA4N    B   // Port
#define PA4P    3   // PinNummer
#define PA5N    D   // Port
#define PA5P    6   // PinNummer
#define PA6N    B   // Port
#define PA6P    4   // PinNummer
#define PA7N    B   // Port
#define PA7P    1   // PinNummer

#define PB0N    B   // Port
#define PB0P    5   // PinNummer
#define PB1N    C   // Port
#define PB1P    0   // PinNummer
#define PB2N    D   // Port
#define PB2P    4   // PinNummer
#define PB3N    C   // Port
#define PB3P    4   // PinNummer
#define PB4N    C   // Port
#define PB4P    2   // PinNummer
#define PB5N    C   // Port
#define PB5P    3   // PinNummer
#define PB6N    D   // Port
#define PB6P    1   // PinNummer
#define PB7N    C   // Port
#define PB7P    1   // PinNummer

///////////////////////////////////////
///////////////////////////////////////

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>   /* benötigt von usbdrv.h */
#include "usbdrv.h"

#define CONCAT(a, b)            a ## b
#define OUTPORT(name)           CONCAT(PORT, name)
#define INPORT(name)            CONCAT(PIN, name)
#define DDRPORT(name)           CONCAT(DDR, name)

uint8_t key_buffer[16];

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB Interface ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM const char usbHidReportDescriptor[22] = {    /// USB Report descriptor
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0xa1, 0x01,                    //   COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           //   END_COLLECTION
};
/* Da wir nur ein Feature Report definieren, verwenden wir keine Report-ID's
 * Dies würde das erste Byte des Berichts sein, der Gesamte Bereich besteht aus
 * 16 Festdefinierten Daten Bytes

 0x00 - 0x07    = PortB Eingangswerte bei jew. gesetztem PortA Ausgangswert (C64 Key Matrix)
 0x08           = RESTORE Status (0 = nicht gedrückt; 1 = gedrückt)
 0x09 - 0x0F    = nicht benutzt (reserviert f. nächste Versione) */

/* Diese Variablen steuern die aktuelle Datenübertragung  */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() wird aufgerufen wenn der Host Daten Empfangen möchte */
uchar usbFunctionRead(uchar *data, uchar len)
{
    uint8_t i;

    if(len > bytesRemaining)
        len = bytesRemaining;

    for(i=0;i<len;i++)
    {
        data[i] = key_buffer[i+currentAddress];
    }
    currentAddress += len;
    bytesRemaining -= len;
    return len;
}

/* ------------------------------------------------------------------------- */

/* usbFunctionWrite() wird aufgerufen wenn der Host Daten Senden möchte */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    return 1;
    /// In dieser Version muss der Host keine Daten senden
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 16;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 16;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

/// Bezeichnug PORTA und PORTB kommt vom C64 ;) ///

void setPORTA(uint8_t value)
{
    // Bit0
    OUTPORT(PA0N) &= ~(0x01<<PA0P);
    OUTPORT(PA0N) |= ((~value >> 0) & 0x01)<<PA0P;
    // Bit1
    OUTPORT(PA1N) &= ~(0x01<<PA1P);
    OUTPORT(PA1N) |= ((~value >> 1) & 0x01)<<PA1P;
    // Bit2
    OUTPORT(PA2N) &= ~(0x01<<PA2P);
    OUTPORT(PA2N) |= ((~value >> 2) & 0x01)<<PA2P;
    // Bit3
    OUTPORT(PA3N) &= ~(0x01<<PA3P);
    OUTPORT(PA3N) |= ((~value >> 3) & 0x01)<<PA3P;
    // Bit4
    OUTPORT(PA4N) &= ~(0x01<<PA4P);
    OUTPORT(PA4N) |= ((~value >> 4) & 0x01)<<PA4P;
    // Bit5
    OUTPORT(PA5N) &= ~(0x01<<PA5P);
    OUTPORT(PA5N) |= ((~value >> 5) & 0x01)<<PA5P;
    // Bit6
    OUTPORT(PA6N) &= ~(0x01<<PA6P);
    OUTPORT(PA6N) |= ((~value >> 6) & 0x01)<<PA6P;
    // Bit7
    OUTPORT(PA7N) &= ~(0x01<<PA7P);
    OUTPORT(PA7N) |= ((~value >> 7) & 0x01)<<PA7P;
}

/* ------------------------------------------------------------------------- */

uint8_t getPORTB()
{
    uint8_t value = 0;

    // Bit0
    value |= ((INPORT(PB0N)>>PB0P)&0x01);
    // Bit1
    value |= ((INPORT(PB1N)>>PB1P)&0x01)<<1;
    // Bit2
    value |= ((INPORT(PB2N)>>PB2P)&0x01)<<2;
    // Bit3
    value |= ((INPORT(PB3N)>>PB3P)&0x01)<<3;
    // Bit4
    value |= ((INPORT(PB4N)>>PB4P)&0x01)<<4;
    // Bit5
    value |= ((INPORT(PB5N)>>PB5P)&0x01)<<5;
    // Bit6
    value |= ((INPORT(PB6N)>>PB6P)&0x01)<<6;
    // Bit7
    value |= ((INPORT(PB7N)>>PB7P)&0x01)<<7;

    return ~value;
}

/* ------------------------------------------------------------------------- */

void LEDOn()
{
    OUTPORT(LED_N) |= 1 << LED_P;
}

/* ------------------------------------------------------------------------- */

void LEDOff()
{
    OUTPORT(LED_N) &= ~(1 << LED_P);
}

/* ------------------------------------------------------------------------- */

void keyPoll(void)
{
    uint8_t BitMask = 0x01;
    uint8_t i = 0;

    setPORTA(0xFF);
    _delay_us(10);

    if(getPORTB() != 0x00)
    {
        /// Mind. eine Taste der 8x8 Matrix ist gedrückt ///
        LEDOn();

        /// Prüfen auf RESTORE ///
        if(~INPORT(RESTORE_N) & (1 << RESTORE_P))
        {
            /// RESTORE ist gedrückt ///
            key_buffer[8] = 0x01;
        }
        else
        {
            /// RESTORE ist nicht gedrückt ///
            key_buffer[8] = 0x00;
        }
    }
    else
    {
        /// Prüfen auf RESTORE ///
        if(~INPORT(RESTORE_N) & (1 << RESTORE_P))
        {
            LEDOn();
            /// RESTORE ist gedrückt ///
            key_buffer[8] = 0x01;
        }
        else
        {
            LEDOff();
            /// RESTORE ist nicht gedrückt ///
            key_buffer[8] = 0x00;
        }
    }

    for(i=0;i<8;i++)
    {
        setPORTA(BitMask);
        _delay_us(10);
        key_buffer[i] = getPORTB();
        BitMask <<= 1;
    }
}

/* ------------------------------------------------------------------------- */

int main(void)
{
    // Port A als Ausgänge setzen
    DDRPORT(PA0N) |= 1 << PA0P;
    DDRPORT(PA1N) |= 1 << PA1P;
    DDRPORT(PA2N) |= 1 << PA2P;
    DDRPORT(PA3N) |= 1 << PA3P;
    DDRPORT(PA4N) |= 1 << PA4P;
    DDRPORT(PA5N) |= 1 << PA5P;
    DDRPORT(PA6N) |= 1 << PA6P;
    DDRPORT(PA7N) |= 1 << PA7P;

    // LED Ausgang setzen
    DDRPORT(LED_N) |= 1 << LED_P;

    // Port B als Eingänge setzen
    DDRPORT(PB0N) &= ~(1 << PB0P);
    DDRPORT(PB1N) &= ~(1 << PB1P);
    DDRPORT(PB2N) &= ~(1 << PB2P);
    DDRPORT(PB3N) &= ~(1 << PB3P);
    DDRPORT(PB4N) &= ~(1 << PB4P);
    DDRPORT(PB5N) &= ~(1 << PB5P);
    DDRPORT(PB6N) &= ~(1 << PB6P);
    DDRPORT(PB7N) &= ~(1 << PB7P);

    // RESTORE Eingang setzen
    DDRPORT(RESTORE_N) &= ~(1 << RESTORE_P);

    // Port B Pullup Widerstände aktivieren
    OUTPORT(PB0N) |= 1 << PB0P;
    OUTPORT(PB1N) |= 1 << PB1P;
    OUTPORT(PB2N) |= 1 << PB2P;
    OUTPORT(PB3N) |= 1 << PB3P;
    OUTPORT(PB4N) |= 1 << PB4P;
    OUTPORT(PB5N) |= 1 << PB5P;
    OUTPORT(PB6N) |= 1 << PB6P;
    OUTPORT(PB7N) |= 1 << PB7P;

    // RESTORE Pullup Widertsand setzen
    OUTPORT(RESTORE_N) |= 1 << RESTORE_P;

    uchar   i;
    // key_buffer löschen
    for(i=0;i<16;i++)
    {
        key_buffer[i] = 0x00;
    }

    // Startmeldung mittels Blinken 5x
    for(i=0;i<5;i++)
    {
        LEDOn();
        _delay_ms(40);
        LEDOff();
        _delay_ms(80);
    }

    // USB Initialisieren
    wdt_enable(WDTO_1S);
    usbInit();
    usbDeviceDisconnect();  // neues auflisten erzwingen, solange der Interrupt deaktiviert ist!
    i = 0;
    while(--i){             // Fake USB Disconnect für > 250 ms
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    sei();

    for(;;){                // Endlosschleife
        wdt_reset();
        usbPoll();
        keyPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
