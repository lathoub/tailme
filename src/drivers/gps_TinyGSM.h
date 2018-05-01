
static void smartdelay(unsigned long ms)
{
}

bool gps_getLocation(float* longitude, float* latitude, float* speed, float* hdop)
{
    float dummy;
    bool fix = modem.getGPS(latitude, longitude, speed);
    return fix;
}

void gps_config()
{
}

void gps_init()
{
    modem.enableGPS();
}

void gps_connect()
{
  float dummy;
  while (!gps_getLocation(&dummy, &dummy, &dummy, &dummy)) {
    smartdelay(100);
  }
}

void gps_maintain(int delay)
{
  smartdelay(delay);
}
