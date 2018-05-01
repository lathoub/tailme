
bool gps_getLocation(float* longitude, float* latitude, float* speed, float* hdop)
{    
  return true;
}

void gps_init()
{
}

void gps_connect()
{
  float dummy;
  while (!gps_getLocation(&dummy, &dummy, &dummy, &dummy)) {
  }
}

void gps_maintain(int delay)
{
}
