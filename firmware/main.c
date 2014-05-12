/* Name: main.c
 * Projekt: lan64usb
 * Author: Thorsten Kattanek
 * Erstellt am: 11.05.2014
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

//#define CONCAT(a, b)            a ## b
//#define OUTPORT(name)           CONCAT(PORT, name)
//#define INPORT(name)            CONCAT(PIN, name)
//#define DDRPORT(name)           CONCAT(DDR, name)

volatile unsigned char buffer[128];
volatile unsigned short led_blink_counter = 0;
volatile unsigned char error_flag = 0;
volatile unsigned char transfer_status = 0;
volatile unsigned short block_anzahl;

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
    0x95, 0x80,                    //   REPORT_COUNT (128)
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
    /*
    uint8_t i;

    if(len > bytesRemaining)
        len = bytesRemaining;

    for(i=0;i<len;i++)
    {
        data[i] = buffer[i+currentAddress];
    }

    currentAddress += len;
    bytesRemaining -= len;
    */
    return len;
}

/* ------------------------------------------------------------------------- */

void transfer_block(void)
{
    unsigned short time_out;
    unsigned char i;

    switch(transfer_status)
    {
    case 0:

        block_anzahl = (buffer[5]+1)<<1;
        transfer_status = 1;
        error_flag = 0;             // Fehler zurücksetzen

        ////////////////////// Syncronisieren mit C64 Start ///////////////////////
        ///////////////////////////////////////////////////////////////////////////

        // C64 PORTB Eingänge auf 0 setzen (PIN 3-7)
        PORTB &= ~0x08;
        PORTD &= ~0xf0;

        // Warten auf C64 PORTB Ausgänge 0 (PIN 0-2)
        time_out = 0xfff;
        while((time_out != 0) && !((PINB & 0x07) == 0x00))
        {
            time_out--;
        }

        if(time_out != 0)
        {
            // C64 PORTB Eingänge auf 1 setzen (PIN 3-7)
            PORTB |= 0x08;
            PORTD |= 0xf0;

            // Warten auf C64 PORTB Ausgänge 1 (PIN 0-2)
            time_out = 0xfff;
            while((time_out != 0) && !((PINB & 0x07) == 0x07))
            {
                time_out--;
            }

            if(time_out != 0)
            {
                // C64 PORTB Eingänge auf 0 setzen (PIN 3-7)
                PORTB &= ~0x08;
                PORTD &= ~0xf0;

                // Warten auf C64 PORTB Ausgänge 0 (PIN 0-2)
                time_out = 0xfff;
                while((time_out != 0) && !((PINB & 0x07) == 0x00))
                {
                    time_out--;
                }

                if(time_out != 0)
                {

                }

                else
                {
                    // Error !
                    transfer_status = 0;
                    error_flag = 1;
                    return;
                }
            }
            else
            {
                // Error !
                transfer_status = 0;
                error_flag = 1;
                return;
            }
        }
        else
        {
            // Error !
            transfer_status = 0;
            error_flag = 1;
            return;
        }

        ////////////////////// Syncronisieren mit C64 Ende ///////////////////////
        ///////////////////////////////////////////////////////////////////////////

        /// 1. 128 Bytes des Headers übertragen ///

        for(i=0;i<128;i++)
        {
            PORTD = (PORTD & 0x0f) | buffer[i] << 4;
            PORTB |= 0x08;

            // Warten auf C64 PORTB PIN2

            time_out = 0xfff;
            while((time_out != 0) && !(PINB & 0x04))
            {
                time_out--;
            }

            if(time_out != 0)
            {
                // HighNibble auf PORT legen
                PORTD = (PORTD & 0x0f) | (buffer[i] & 0xf0);
                PORTB &= ~0x08;

                time_out = 0xfff;
                while((time_out != 0) && (PINB & 0x04))
                {
                    time_out--;
                }

                if(time_out != 0)
                {
                    // Alles OK !!
                }
                else
                {
                    // Error !
                    transfer_status = 0;
                    error_flag = 1;
                    break;
                }
            }
            else
            {
                // Error !
                transfer_status = 0;
                error_flag = 1;
                break;
            }
        }
        block_anzahl--;
    break;

    case 1:
        for(i=0;i<128;i++)
        {
            PORTD = (PORTD & 0x0f) | buffer[i] << 4;
            PORTB |= 0x08;

            // Warten auf C64 PORTB PIN2

            time_out = 0xfff;
            while((time_out != 0) && !(PINB & 0x04))
            {
                time_out--;
            }

            if(time_out != 0)
            {
                // HighNibble auf PORT legen
                PORTD = (PORTD & 0x0f) | (buffer[i] & 0xf0);
                PORTB &= ~0x08;

                time_out = 0xfff;
                while((time_out != 0) && (PINB & 0x04))
                {
                    time_out--;
                }

                if(time_out != 0)
                {
                    // Alles OK !!
                }
                else
                {
                    // Error !
                    transfer_status = 0;
                    error_flag = 1;
                }
            }
            else
            {
                // Error !
                transfer_status = 0;
                error_flag = 1;
            }
        }

        block_anzahl--;
        if(block_anzahl == 0) transfer_status = 0;

    break;
    }
}
/* ------------------------------------------------------------------------- */

/* usbFunctionWrite() wird aufgerufen wenn der Host Daten Senden möchte */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0) return 1;                       /* end of transfer */

    if(len > bytesRemaining) len = bytesRemaining;

    uchar i;

    for(i=0;i<len;i++)
    {
        buffer[currentAddress+i] = data[i];
    }

    currentAddress += len;
    bytesRemaining -= len;

    if(bytesRemaining == 0)
    {
        // Letztes Paket erhalten
        transfer_block();
    }

    return bytesRemaining == 0; /* return 1 if this was the last chunk */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

void error_poll(void)
{
    if(error_flag == 0) return;
    if(led_blink_counter & 0x8000)
        PORTD |= 0x01;
    else PORTD &= 0xfe;

    led_blink_counter++;
}

int main(void)
{
    // PortB Pin 3 auf Ausgang setzen
    DDRB |= 0x08;

    // PortD Pin 4-7 und Pin 0 auf Ausgang setzen
    DDRD |= 0xf1;

    PORTB = 0;
    PORTD &= 0x0f;

    uchar   i;
    // key_buffer löschen
    for(i=0;i<2;i++)
    {
        buffer[i] = 0x00;
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
        error_poll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
