#ifdef TINYGSM

#if defined SIM800
#define TINY_GSM_MODEM_SIM800
#elif defined SIM808
#define TINY_GSM_MODEM_SIM808
#elif defined SARAU201
#define TINY_GSM_MODEM_U201
#endif

#include <TinyGsmClient.h>

#if defined DEBUG
#define DUMP_AT_COMMANDS
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);

#endif

void OnPowerOn(void* sender, EventArgs* e)
{
    PowerOn();
}

void OnModemReady(void* sender, EventArgs* e)
{
    ModemReady;
}

void gsm_setBaud(int baud)
{
    modem.setBaud(baud);
}

void gsm_init()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Set GSM module baud rate
  SerialAT.begin(19200);

  modem.PowerOn    += OnPowerOn;
  modem.ModemReady += OnModemReady;

  modem.init();
}

void gsm_connect()
{
  DEBUG_PRINT("Waiting for network registration");
  while (!modem.waitForNetwork())
  {
    delay(1000);
  }
  DEBUG_PRINTLN("\nRegsitered on the network");

  DEBUG_PRINT("Connecting to GPRS");
  while (!modem.gprsConnect(SECRET_GPRS_APN, SECRET_GPRS_LOGIN, SECRET_GPRS_PASSWORD))
  {
    DEBUG_PRINT(F("."));
    delay(100);
  }
  DEBUG_PRINTLN("\nConnected to GPRS");
    
  digitalWrite(LED_BUILTIN, HIGH);
}

int8_t gsm_getRSSI()
{
    return (int8_t)modem.getSignalQuality();
}

const char* gsm_getImei()
{
  return modem.getIMEI().c_str();
}

void gsm_signalAlive(String& phoneNumber, String& message)
{
//  modem.sendSMS(phoneNumber.c_str(), message.c_str());
}

void gsm_signalLocation(String& phoneNumber, float lng, float lat)
{
  char slat[12];
  char slng[12];
  dtostrf(lat, 6, 6, slat);
  dtostrf(lng, 6, 6, slng);

  char sBuffer[60];
  sprintf(sBuffer, "https://maps.google.com/?q=%s,%s", slng, slat);

//  modem.sendSMS(phoneNumber.c_str(), sBuffer);
}

void gsm_maintain()
{
}

