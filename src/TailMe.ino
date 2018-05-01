
#define RELEASE

// Boards
//#define SIMDUINO
//#define FEATHER32U4FONA
#define MKR1400
//#define LEONARDO
//#define MEGA

// Shields
//#define SIM808
//#define MTK3339

// Drivers
//#define FONA
#define TINYGSM
#define TINYGPS
#define HTTP_CLIENT

#include "tailme_config.h"
#include "gsm_secrets.h"
#include "hardware_config.h"
#include "software_config.h"

char vin[17 + 1];
float gpsLongitude, gpsLatitude, gpsHdop;
int16_t gpsSpeed;

// Forware declaration
void tailMe(bool startOfLeg = 0);

void setup()
{
  unsigned long startTime = millis();

#ifdef DEBUG
  // initialize serial communications and wait for port to open:
  SerialMon.begin(115200);
  while (!SerialMon) {}
#endif
  DEBUG_PRINTLN(F("Booting"));

  hardware_init();

  DEBUG_PRINTLN(F("Connecting GSM"));
  gsm_init();
  gsm_connect();
  DEBUG_PRINTLN(F("Connected GSM"));

  digitalWrite(LED_BUILTIN, HIGH);

  DEBUG_PRINT("imei: ");
  DEBUG_PRINTLN(gsm_getImei());

  strcpy(vin, "");
  if (http_get(getVinGET(buffer33, gsm_getImei())))
  {
    int statusCode = http_getStatusCode();

    DEBUG_PRINT("statusCode: ");
    DEBUG_PRINTLN(statusCode);

    if (statusCode == 200 || statusCode == 201) {
      char* response = http_getResponse();

      const char* key   = strtok(response, ":");
      const char* value = strtok(NULL, "}");

      strncpy(vin, value + 1, 17);
    }
  }

  if (strlen(vin) != 17) {
    // TODO: TURN ERROR LED ON
    DEBUG_PRINTLN("unable to get VIN. Not sure what to do");
    while (true) {}
  }

  digitalWrite(LED_BUILTIN, LOW);

  DEBUG_PRINT("vin: ");
  DEBUG_PRINTLN(vin);

  DEBUG_PRINT(F("Startup time (in ms): "));
  DEBUG_PRINTLN((millis() - startTime));

  DEBUG_PRINTLN(F("Connecting GPS"));
  gps_init();
  gps_connect();
  DEBUG_PRINTLN(F("Connected GPS"));
  gps_getLocation(&gpsLongitude, &gpsLatitude, &gpsSpeed, &gpsHdop);

  // and post the first step of the leg
  tailMe(true);
}

void loop()
{
  if (gps_getLocation(&gpsLongitude, &gpsLatitude, &gpsSpeed, &gpsHdop))
  {
    if (gpsSpeed > -2.0f)
    {
      tailMe();
    }
    else
    {
      // NO-ACTIVITY LED
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      DEBUG_PRINTLN(F("driving too slow"));
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  gsm_maintain();

  // database datetime stamp in database has 1s resolution
  // and should be unique.
  // Waiting for 1100ms garantees uniqueness
  gps_maintain(1150);
}


void tailMe(bool startOfLeg)
{
  digitalWrite(LED_BUILTIN, HIGH);

  if (http_post(getAddLocationPath(buffer22, vin),
                "application/json",
                getGeoJSONPoint(buffer33, gpsLongitude, gpsLatitude, gpsSpeed, gsm_getRSSI(), gpsHdop, startOfLeg)))
  {
    DEBUG_PRINTLN("Awaiting response");
    int statusCode = http_getStatusCode();
    DEBUG_PRINT("statusCode: ");
    DEBUG_PRINTLN(statusCode);
    if (statusCode > 0) {
      String response = http_getResponse();
      DEBUG_PRINT("response: ");
      DEBUG_PRINTLN(response);
    }
  }
  else
  {
    // ERROR LED
    DEBUG_PRINTLN(F("error during HTTP"));
  }

  digitalWrite(LED_BUILTIN, LOW);
}
