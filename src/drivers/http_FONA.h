
uint16_t _statusCode;
int http_getStatusCode()
{
    return _statusCode;
}

String _response;
String http_getResponse()
{
    return _response;
}

const String urlTemplate = "http://%1:%2%3";

int http_get(String& urlPath)
{
    String url = urlTemplate;
    url.replace("%1", server);
    url.replace("%2", String(port).c_str());
    url.replace("%3", urlPath);
    
    DEBUG_PRINTLN("URL");
    DEBUG_PRINTLN(url);

    int16_t length;
    int succes = fona.HTTP_GET_start(url.c_str(), &_statusCode, (uint16_t *)&length);
    if (!succes)
        return false;

    _response = "";
    while (length > 0) {
        while (fona.available()) {
            char c = fona.read();
            
            _response += c;

            length--;
            if (! length) break;
        }
    }
    fona.HTTP_GET_end();
    
    return _statusCode;
}

int http_post(String& urlPath, String& contentType, String& data)
{
    String url = urlTemplate;
    url.replace("%1", server);
    url.replace("%2", String(port).c_str());
    url.replace("%3", urlPath);

    DEBUG_PRINTLN("URL");
    DEBUG_PRINTLN(url);

  int16_t length;
  bool succes = fona.HTTP_POST_start(url.c_str(), F("application/json"), (uint8_t*) data.c_str(), data.length(), &_statusCode, (uint16_t *)&length);
    if (!succes)
        return false;
    
    _response = "";
    while (length > 0) {
        while (fona.available()) {
            char c = fona.read();
            
            _response += c;
            
            length--;
            if (! length) break;
        }
    }

    fona.HTTP_POST_end();

  return _statusCode;
}
