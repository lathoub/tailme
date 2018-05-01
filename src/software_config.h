#include "util/buffers.h"
#include "util/bufferMaker.h"

#if defined TINYGSM
#include "drivers/gsm_TinyGSM.h"
#elif defined FONA
#include "drivers/gsm_FONA.h"
#elif defined MKRGSM
#include "drivers/gsm_MKRGSM.h"
#endif

#ifdef TINYGPS
#include "drivers/gps_TinyGPS.h"
#elif defined TINYGSM_GPS
#include "drivers/gps_TinyGSM.h"
#elif defined FONA_GPS
#endif

#ifdef HTTP_FONA
#include "drivers/http_FONA.h"
#elif defined HTTP_TCP
#include "drivers/http_FONA_tcp.h"
#elif defined HTTP_CLIENT
#include "drivers/http_client.h"
#elif defined ARDUINO_HTTP_CLIENT
#include "drivers/http_httpclient.h"
#endif
