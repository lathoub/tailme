#include <ArduinoHttpClient.h>

HttpClient http(client, server, port);

int http_getStatusCode()
{
    return http.responseStatusCode();
}

String http_getResponse()
{
    return http.responseBody();
}

bool http_get(String path)
{
    return http.get(path);
}

bool http_post(String path, String contentType, String body)
{
    return http.post(path, contentType, body);
}
