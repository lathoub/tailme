#include <TinyGPS.h>

TinyGPS gps;

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (serialGPS.available()) {
      gps.encode(serialGPS.read());
    }
  } while (millis() - start < ms);
}

bool gps_getLocation(float* longitude, float* latitude, int16_t* speed = 0, float* hdop = 0)
{    
    *speed = 0;
    *hdop = 0;
        
    gps.f_get_position(longitude, latitude);
  if (*latitude == TinyGPS::GPS_INVALID_F_ANGLE)
    return false;
    
  *speed = (int16_t)gps.f_speed_kmph();
  *hdop  = gps.hdop() / 100.0f;

  return true;
}

void gps_init()
{
    serialGPS.begin(9600);
}

void gps_connect()
{
  float dummy;
  while (!gps_getLocation(&dummy, &dummy)) {
    smartdelay(100);
  }
}

void gps_maintain(int delay)
{
  smartdelay(delay);
}
