#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4
#define FONA_RI  7

#include <SoftwareSerial.h>
SoftwareSerial SerialAT(FONA_TX, FONA_RX); // RX, TX

// GPS comms
#define serialGPS Serial1

void hardware_init()
{
}

void PowerOn()
{
}

void ModemReady()
{
}
