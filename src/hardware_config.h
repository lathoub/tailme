// Boards

#ifdef DEBUG
// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// need to do some debugging...
#define DEBUG_PRINT(...)    SerialMon.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)    SerialMon.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif

#ifdef MKR1400
#include "boards/MKR1400_config.h"
#elif defined SIMDUINO
#include "boards/SIMduino_config.h"
#elif defined  FEATHER32U4FONA
#include "boards/Feather32u4FONA_config.h"
#elif defined  LEONARDO
#include "boards/Leonardo_config.h"
#elif defined  MEGA
#include "boards/Mega_config.h"
#endif
