#include <Arduino.h>
#include <wiring_private.h>
#include <avr/dtostrf.h>

#define SerialAT SerialGSM

#define SARAU201

// Forward declaration in gsm_*
void gsm_setBaud(int);

// Create the new UART instance assigning it to pin 0 and 1
Uart serialGPS (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0);

void hardware_init()
{
    pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
    pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1
}

void PowerOn()
{
    // FOR THE MKR GSM 1400
    // reset / powerup the modem
    pinMode(GSM_RESETN, OUTPUT);
    digitalWrite(GSM_RESETN, HIGH);
    delay(100);
    digitalWrite(GSM_RESETN, LOW);
}

void ModemReady()
{
    gsm_setBaud(921600);
    
    SerialAT.end();
    delay(100);
    SerialAT.begin(921600);
}

// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
   serialGPS.IrqHandler();
}
