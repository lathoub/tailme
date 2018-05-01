enum ContentType {
    unknown,
    textPlain,
    textHtml,
    applicationJson,
    applicationXml
};

const static char pm_contentLength[] PROGMEM = "Content-Length: ";
const static char pm_contentType[] PROGMEM = "Content-Type: ";
const static char pm_host[] PROGMEM = "Host: ";
const static char pm_crlf[] PROGMEM = "\r\n";
const static char pm_http11[] PROGMEM = "HTTP/1.1";
const static char pm_application_json[] PROGMEM = "application/json";
const static char pm_text_plain[] PROGMEM = "text/plain";
const static char pm_connection[] PROGMEM = "Connection: ";
const static char pm_keepAlive[] PROGMEM = "keep-alive";
const static char pm_close[] PROGMEM = "close";
const static char pm_post[] PROGMEM = "POST ";
const static char pm_get[] PROGMEM = "GET ";

const char sc[2] = " ";
const char s[2] = ":";
int contentLength = 0;
ContentType contentType;
char buffer[100] = {0};
bool keepAlive = false;
unsigned long timeout = 5000;

char * trim(char * s)
{
    char * p = s;
    int l = strlen(p);
    
    while (isspace(p[l - 1])) p[--l] = 0;
    while ( * p && isspace( * p)) ++p, --l;
    
    memmove(s, p, l + 1);
    return s;
}

void parseHeader(char * buffer)
{
    char * key = strtok(buffer, " ");
    char * value = strtok(NULL, "\n");
    
    DEBUG_PRINT("key: ");
    DEBUG_PRINTLN(key);
    DEBUG_PRINT("value: ");
    DEBUG_PRINTLN(value);
    
    // Content-Length:
    if (strncmp_P(key, pm_contentLength, strlen_P(pm_contentLength) - 1) == 0) {
        contentLength = atoi(value);
    }
    
    // Content-Type:
    if (strncmp_P(key, pm_contentType, strlen_P(pm_contentType) - 1) == 0) {
        
        contentType = unknown;
        if (strncmp_P(value, pm_application_json, strlen_P(pm_application_json)) == 0)
            contentType = applicationJson;
        else if (strncmp_P(value, pm_text_plain, strlen_P(pm_text_plain)) == 0)
            contentType = textPlain;
    }
    
    // Connection:
    if (strncmp_P(key, pm_connection, strlen_P(pm_connection) - 1) == 0) {
       keepAlive = strncmp_P(value, pm_keepAlive, strlen_P(pm_keepAlive)) == 0;
    }
}

int http_getStatusCode()
{
    unsigned long timer = millis();
    while (client.connected() && millis() - timer < timeout) {
        int bytesRead = client.readBytesUntil('\n', buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            DEBUG_PRINT("----------- bytesRead <= 0");
            continue;
        }
        buffer[bytesRead] = '\0';
        
        if (strncmp_P(buffer, pm_http11, strlen_P(pm_http11)) != 0)
            return -1;

        const char* protocol   = strtok(buffer, " ");
        const char* statusCode = strtok(NULL, " ");
        const char* status     = strtok(NULL, " ");

        DEBUG_PRINT("protocol: ");
        DEBUG_PRINTLN(protocol);
        DEBUG_PRINT("statusCode: ");
        DEBUG_PRINTLN(statusCode);
        DEBUG_PRINT("status: ");
        DEBUG_PRINTLN(status);

        return atoi(statusCode);
    }
    
    // did we timeout?
    if (millis() - timer > timeout) {
        client.stop();
        return -2;
    }
    
    return -3; // something else
}

char * http_getResponse()
{
    unsigned long timer = millis();
    while (client.connected() && millis() - timer < timeout) {
        int bytesRead = client.readBytesUntil('\n', buffer, sizeof(buffer));
        if (bytesRead < 2)
            break;
        buffer[bytesRead] = '\0';
        
        parseHeader(buffer);
    }
    
    // did we timeout?
    if (millis() - timer > timeout) {
        client.stop();
        return "";
    }
    
    // TODO: contentLength > bufferLength
    int bytesRead = client.readBytes(buffer, contentLength);
    buffer[bytesRead] = '\0';
    
    if (bytesRead != contentLength) {
        client.stop();
        return "";
    }
    
    // empty everything else in the buffer
    while (client.available())
        client.read();
    
    // Server indicates we can keep the line open
    if (!keepAlive)
        client.stop();
    
    return buffer;
}

int http_connect()
{
    if (!client.connected()) {
        if (!client.connect(server, port)) {
            return false;
        }
        return true;
        
    } else {
        return true;
    }
}

bool http_get(const char * path)
{
    if (!http_connect()) {
        client.stop();
        return false;
    }
    
    strcpy_P(buffer44, pm_get);
    strcat(buffer44, path);
    strcat(buffer44, " ");
    strcat_P(buffer44, pm_http11);
    strcat_P(buffer44, pm_crlf);

    strcat_P(buffer44, pm_host);
    strcat(buffer44, server);
    strcat(buffer44, ":");
    strcat(buffer44, itoa(port, fb, 10));
    strcat_P(buffer44, pm_crlf);
    strcat_P(buffer44, pm_connection);
    strcat_P(buffer44, pm_close);
    strcat_P(buffer44, pm_crlf);
    strcat_P(buffer44, pm_crlf);
    
    client.print(buffer44);
    client.flush();
    
    /*
     // WHEN MEMORY IS REALLY TIGHT!!
     
     client.print("GET ");
     client.print(path);
     client.println(" HTTP/1.1");
     
     client.print("Host: ");
     client.print(server);
     client.print(":");
     client.println(port);
     
     client.println("Connection: close");
     client.println();
     */
    
    return true;
}

bool http_post(const char * path,
               const char * contentType,
               const char * body)
{
    if (!http_connect()) {
        client.stop();
        return false;
    }
    
    strcpy_P(buffer44, pm_post);
    strcat(buffer44, path);
    strcat(buffer44, " ");
    strcat_P(buffer44, pm_http11);
    strcat_P(buffer44, pm_crlf);

    strcat_P(buffer44, pm_host);
    strcat(buffer44, server);
    strcat(buffer44, ":");
    strcat(buffer44, itoa(port, fb, 10));
    strcat_P(buffer44, pm_crlf);
    
    strcat_P(buffer44, pm_connection);
    strcat_P(buffer44, pm_keepAlive);
    strcat_P(buffer44, pm_crlf);
    
    strcat_P(buffer44, pm_contentType);
    strcat(buffer44, contentType);
    strcat_P(buffer44, pm_crlf);
    
    strcat_P(buffer44, pm_contentLength);
    strcat(buffer44, itoa(strlen(body), fb, 10));
    strcat_P(buffer44, pm_crlf);
    strcat_P(buffer44, pm_crlf);
    
    strcat(buffer44, body);
    
    client.print(buffer44);
    /*
     // SEND EVERY PART SEPERATELY, MORE MEMORY EFFICIANT
     // WHEN MEMORY IS REALLY TIGHT!!
     
     // Make a HTTP request:
     client.print("POST ");
     client.print(path);
     client.println(" HTTP/1.1");
     
     // Headers
     client.print("Host: ");
     client.print(server);
     client.print(":");
     client.println(port);
     
     client.println("Connection: keep-alive");
     
     client.print("Content-Type: ");
     client.println(contentType);
     client.print("Content-Length: ");
     client.println(body.length());
     
     client.println();
     
     client.println(body);
     */
    client.flush();
    
    return true;
}
