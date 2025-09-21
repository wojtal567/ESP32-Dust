#pragma once

#include <WebServer.h>
#include <ArduinoJson.h>

namespace HttpApi {
    void setAppIp();
    void handleNotFound();
    
    void setupRouting();
}