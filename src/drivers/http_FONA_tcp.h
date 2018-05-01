
#define MQTT_FONA_INTERAVAILDELAY 100
#define MQTT_FONA_QUERYDELAY 500

static char message[400];

// keepAlive = true will keep the socket open, so it doesn't need to be opened for the next measurement.
// (a check is made before each send to see if the socket is still open, if not, if will open it.
// keeping the socket alive reduces service load and keep the amount of bytes transmitted lower as well.
static bool keepAlive = true;

int http_get(String urlPath)
{
    uint16_t statusCode = 200;
    
    if (!fona.TCPconnected())
    {
        DEBUG_PRINT(F("TCP connecting: "));
        DEBUG_PRINT(server);
        DEBUG_PRINT(F(" on port "));
        DEBUG_PRINTLN(port);
        
        // try multiple times?
        bool succes = fona.TCPconnect(server, port);
        if (!succes) {
            DEBUG_PRINTLN("FAILED TO TCPconnect");
            return -1;
        }
    }
    
    for (int i = 0; i < sizeof(message); i++)
        message[i] = '\0';

    strcpy(message , "GET ");
    strcat(message , urlPath.c_str());
    strcat(message , " HTTP/1.1");
    strcat(message , "\r\n");

    strcat(message , "Host: ");
    strcat(message , server);
    if (port != 80) {
        strcat(message , ":");
        strcat(message , String(port).c_str());
    }
    strcat(message , "\r\n");

    bool sendSucces = fona.TCPsend(message, strlen(message));
    if (sendSucces)
    {
        int bytesRead = fona.readline(8000, false);
        DEBUG_PRINTLN(fona.replybuffer);
        
        const char s[2] = " ";
        char* token = strtok(fona.replybuffer, s);
        token = strtok(NULL, s);
        
        statusCode = atoi(token);
        DEBUG_PRINTLN(token);
   }
    
    fona.TCPclose(); // just to make sure, also depends on the Connection parameter
    
    return statusCode;
}

int http_post(String urlPath, String contentType, String data)
{
  uint16_t statusCode = 200;

  if (!fona.TCPconnected())
  {
    DEBUG_PRINT(F("TCP connecting: "));
    DEBUG_PRINT(server);
    DEBUG_PRINT(F(" on port "));
    DEBUG_PRINTLN(port);

    // try multiple times?
    bool succes = fona.TCPconnect(server, port);
    if (!succes) {
      DEBUG_PRINTLN("FAILED TO TCPconnect");
      return -1;
    }
  }

  for (int i = 0; i < sizeof(message); i++)
    message[i] = '\0';

  strcpy(message , "POST ");
  strcat(message , urlPath);
  strcat(message , " HTTP/1.1");
  strcat(message , "\r\n");

  // DefaultRequestHeaders
  strcat(message , "Host: ");
  strcat(message , server);
  if (port != 80) {
    strcat(message , ":");
    strcat(message , String(port).c_str());
  }
  strcat(message , "\r\n");

  strcat(message , "User-Agent: Arduino/2.2.0");
  strcat(message , "\r\n");

  // Tell the server to
  // close this connection after we're done
  strcat(message , "Connection: ");
  if (keepAlive)
    strcat(message , "keep-alive");
  else
    strcat(message , "close");

  strcat(message , "\r\n");

  strcat(message , "Content-Type: ");
  strcat(message , contentType);
  strcat(message , "\r\n");

  strcat(message , "Accept: */*");
  strcat(message , "\r\n");

  strcat(message , "Content-Length: ");
  strcat(message , String(strlen(data)).c_str());
  strcat(message , "\r\n");

  // end of header
  strcat(message , "\r\n");

  strcat(message , data);
  strcat(message , "\r\n");
  strcat(message , "\r\n");

  strcat(message , "\0"); // for strlen to work

  DEBUG_PRINT(F("Data: "));
  DEBUG_PRINTLN(data);
  DEBUG_PRINT(F("Data Len: "));
  DEBUG_PRINTLN(strlen(data));

  bool sendSucces = fona.TCPsend(message, strlen(message));
  if (sendSucces)
  {
    int bytesRead = fona.readline(8000, false);
    DEBUG_PRINTLN(fona.replybuffer);

    const char s[2] = " ";
    char* token = strtok(fona.replybuffer, s);
    token = strtok(NULL, s);

    statusCode = atoi(token);
    DEBUG_PRINTLN(token);
  }

  if (!keepAlive)
    fona.TCPclose(); // just to make sure, also depends on the Connection parameter

  return statusCode;
}
