
char* getVinGET(char* buffer, const char* imei)
{
    strcpy(buffer, "/sensor/");
    strcat(buffer, imei);
    strcat(buffer, "/getVIN");
    
    return buffer;
}

char* getGeoJSONPoint(char* buffer, float lon, float lat, int16_t speed, int8_t rssi, float hdop, bool firstTime = false)
{
    strcpy(buffer, "{\"type\":\"Feature\",\"geometry\":{\"type\":\"Point\",\"coordinates\":[");
    strcat(buffer, dtostrf(lat, 6,6, fb));
    strcat(buffer, ",");
    strcat(buffer, dtostrf(lon, 6,6, fb));
    strcat(buffer, "]},\"properties\":{");

        strcat(buffer, "\"speed\":");
        strcat(buffer, itoa(speed, fb, 10));

        strcat(buffer, ",\"rssi\":");
        strcat(buffer, itoa(rssi, fb, 10));

        strcat(buffer, ",\"hdop\":");
        strcat(buffer, dtostrf(hdop, 0,2, fb));

        if (firstTime)
            strcat(buffer, ",\"start\":1");

    strcat(buffer, "}}");
    
    return buffer;
}

char* getAddLocationPath(char* buffer, const char* vin)
{
    strcpy(buffer, "/vehicle/");
    strcat(buffer, vin);
    strcat(buffer, "/addlocation");
    
    return buffer;
}
