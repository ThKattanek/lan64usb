/* Name: main.c
 * Projekt: lan64usb
 * Author: Thorsten Kattanek
 * Erstellt am: 10.05.2014
 * Copyright: Thorsten Kattanek
 * Vesrion: 1.00
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 *
 */

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
    /*
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
    */
}

/* ------------------------------------------------------------------------- */

uint8_t getPORTB()
{
    uint8_t value = 0;

    /*
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
    */
    return ~value;
}

void keyPoll(void)
{
    static uint8_t wert = 0;

    PORTB = (PORTB & 0xf0) | (wert & 0x0f);
    PORTD = (PORTD & 0x0f) | (wert & 0xf0);

    wert ++;

    /*
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
    */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
    // PortB Pin 0-3 auf Ausgan setzen
    DDRB |= 0x0f;

    // PortD Pin 4-7 und Pin 0 auf Ausgang setzen
    DDRD |= 0xf1;

    PORTB = 0;
    PORTD &= 0x0f;

    uchar   i;
    // key_buffer löschen
    for(i=0;i<16;i++)
    {
        key_buffer[i] = 0x00;
    }

    // Startmeldung mittels Blinken 5x
    for(i=0;i<5;i++)
    {
        PORTD |= 0x01;
        _delay_ms(40);
        PORTD &= 0xfe;
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
