#include <MKRGSM.h>
#include <avr/dtostrf.h>

// initialize the library instance
GSMClient client;
GPRS gprs;
#ifdef DEBUG
GSM gsmAccess(true);
#else
GSM gsmAccess;
#endif
GSMModem gsmModem;
GSM_SMS sms;
GSMScanner gsmScanner;
GSMBand gsmBand;

// timeout
const unsigned long __TIMEOUT__ = 10 * 5000;

void gsm_init()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
    
  // FOR THE MKR GSM 1400
  // reset / powerup the modem
  pinMode(GSM_RESETN, OUTPUT);
  digitalWrite(GSM_RESETN, HIGH);
  delay(100);
  digitalWrite(GSM_RESETN, LOW);
}

void gsm_connect()
{
  gsmAccess.noLowPowerMode();

  boolean connected = false;
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin(SECRET_PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(SECRET_GPRS_APN, SECRET_GPRS_LOGIN, SECRET_GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      //    SerialMon.println("Not connected");
      delay(100);
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

String gsm_getImei()
{
    return gsmModem.getIMEI();
}

uint8_t gsm_getRSSI()
{
    String signalStrength = gsmScanner.getSignalStrength();
    return atoi(signalStrength.c_str());
}

void gsm_signalAlive(String& phoneNumber, String& message)
{
  sms.beginSMS(phoneNumber.c_str());
  sms.print(message.c_str());
  sms.endSMS();
}

void gsm_signalLocation(String& phoneNumber, float lng, float lat)
{
  char slat[12];
  char slng[12];
  dtostrf(lat, 6, 6, slat);
  dtostrf(lng, 6, 6, slng);

  char sBuffer[60];
  sprintf(sBuffer, "https://maps.google.com/?q=%s,%s", slng, slat);

  sms.beginSMS(phoneNumber.c_str());
  sms.print(sBuffer);
  sms.endSMS();
}

void gsm_maintain()
{
}
