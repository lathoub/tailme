#include <Adafruit_FONA.h>

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void gsm_init()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  SerialAT.begin(19200);

begin:
    
  if (!fona.begin(SerialAT)) {
    DEBUG_PRINT("FONA not found");
      goto begin;
  }

  DEBUG_PRINTLN("FONA found");

    // we need a minimum of battery to run
    uint16_t batt = 0;
    bool succes = fona.getBattVoltage(&batt);
    DEBUG_PRINTLN(batt);
    if (!succes || batt < 3500) {
        DEBUG_PRINTLN("UNDER VOLTAGE - REBOOTING ");
       delay(1000);
        goto begin;
    }
}

void gsm_connect()
{
  DEBUG_PRINT("Waiting for network registration");

  uint8_t networkStatus = fona.getNetworkStatus();

  while (networkStatus != 1 && networkStatus != 5)
  {
    DEBUG_PRINT(F("."));
    delay(100);
    networkStatus = fona.getNetworkStatus();
  }
  DEBUG_PRINTLN("\nRegistered on the network");

  fona.setGPRSNetworkSettings(F("telenetwap.be"), F(""), F(""));

  DEBUG_PRINT("Connecting to GPRS");
    bool needToConnectToGPRS = false;
  while (!fona.GPRSstate())
  {
    needToConnectToGPRS = true;

    DEBUG_PRINT(F("."));
    delay(100);
  }

  if (needToConnectToGPRS)
     fona.enableGPRS(true);

  DEBUG_PRINTLN("\nConnected to GPRS");

  digitalWrite(LED_BUILTIN, HIGH);
}

int gsm_getRSSI()
{
    return fona.getRSSI();
}

String gsm_getImei()
{
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);

  return String(imei);
}

void gsm_signalAlive(String& phoneNumber, String& message)
{
  fona.sendSMS(phoneNumber.c_str(), message.c_str());
}

void gsm_signalLocation(String& phoneNumber, float lng, float lat)
{
  char slat[12];
  char slng[12];
  dtostrf(lat, 6, 6, slat);
  dtostrf(lng, 6, 6, slng);

  char sBuffer[60];
  sprintf(sBuffer, "https://maps.google.com/?q=%s,%s", slng, slat);

  fona.sendSMS(phoneNumber.c_str(), sBuffer);
}

void gsm_maintain()
{
}
