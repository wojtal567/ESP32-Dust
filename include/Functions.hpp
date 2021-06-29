#include <GlobalVariables.hpp>

void notify(String text)
{
    socket.textAll(text);
}

//Get single sample and set text
void getSampleFunc()
{
    Serial.print("getsample func on core: ");
    Serial.println(xPortGetCoreID());
    if (pmsSensor->readData())
    {
        isSampleTaking = true;
        Serial.println("dupskochujowe");
        Serial.println("Succesfully read data from dust sensor.");
        std::map<std::string, float> tmpData = pmsSensor->returnData();
        pmsSensor->dumpSamples();
        data = tmpData;
        sht30.get();    
        // config.currentSampleNumber++;
        temp = sht30.cTemp;
        humi = sht30.humidity;
        Serial.print("Temperature: "); Serial.print(temp);
        Serial.print("\nHumidity: "); Serial.print(humi);
        Serial.println();
        bool running = Rtc.GetIsRunning();
        Serial.println(running);
        if(running)
        {
            Serial.print("datetime for database: ");
            Serial.println(currentDateTime);
            Serial.print("datetime from rtc: ");
            // Serial.println(Rtc.GetDateTime());
            currentDateTime = Rtc.GetDateTime();
            lastSampleTimestamp = getMainTimestamp(currentDateTime);
            Serial.print("lastSampleTimestamp przed wrzuceniem do bazy: " + lastSampleTimestamp);
            mySDCard.save(data, temp, humi, lastSampleTimestamp, &sampleDB, &Serial);
           
            StaticJsonDocument<600> measuredata;
            JsonArray lastRecordToCheck = measuredata.to<JsonArray>();
            mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecordToCheck);
            String output;
            serializeJson(measuredata, output);
            output = output.substring(1, output.length()-1);
            Serial.println(output);
            notify(output);//send data to website
            digitalWrite(FAN_PIN, LOW);
            measureSaveDateTime = currentDateTime;
        }
        else
            Serial.println("RTC is not running, not saving");
        isSampleTaking = false;
    }
}

void config_time()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        for (int i = 0; i < 500; i++)
            dateTimeClient.update();
        configTime(Rtc, dateTimeClient);
        Serial.println("Succesfully updated time on RTC.");
    }
}

bool isLastSampleSaved()
{
    StaticJsonDocument<600> docA;
    JsonArray lastRecordToCheck = docA.to<JsonArray>();
    mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecordToCheck);
    Serial.print("Global: ");
    Serial.print(lastSampleTimestamp);
    Serial.print(" Baza: ");
    Serial.print(lastRecordToCheck[0]["timestamp"].as<String>());
    if (lastSampleTimestamp == lastRecordToCheck[0]["timestamp"].as<String>())
    {
        Serial.println("Last sample has been saved correctly - return true.");
        return true;
    }
    else
    {
        Serial.println("Something went wrong saving last sample - return false");
        return false;
    }
}

//for request from website
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) { 
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        StaticJsonDocument<600> measuredata;
        JsonArray lastRecordToCheck = measuredata.to<JsonArray>();
        mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecordToCheck);
        String output;
        serializeJson(measuredata, output);
        output = output.substring(1, output.length()-1);
        Serial.println(output);
        notify(output);//send data to website
    }
}

//on websocket event
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
            break;
        case WS_EVT_ERROR:
            break;
    }
}

//init of websocket
void initWebSocket() {
    socket.onEvent(onEvent);
    server.addHandler(&socket);
}

void initWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println(xPortGetCoreID());
        request->send(SD, "/web/index.html", "text/html");
    });
    server.on("/js/dashboard.js", HTTP_GET, [](AsyncWebServerRequest *request){
Serial.println(xPortGetCoreID());

        request->send(SD, "/web/js/dashboard.js", "text/javascript");
    });
    server.serveStatic("/index.html", SD, "/web/index.html");
    
    server.serveStatic("/charts.html", SD, "/web/charts.html");
    server.serveStatic("/table.html", SD, "/web/table.html");
    server.serveStatic("/css/styles.css", SD, "/web/css/styles.css");
    server.on("/js/charts.js", HTTP_GET, [](AsyncWebServerRequest *request){
                Serial.println(xPortGetCoreID());

        request->send(SD, "/web/js/charts.js", "text/javascript");
    });
    // server.serveStatic("/js/charts.js", SD, "/web/js/charts.js");
    server.serveStatic("/src/chart.min.js", SD, "/web/src/chart.min.js");
    server.on("/src/all.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
                Serial.println(xPortGetCoreID());

        request->send(SD, "/web/src/all.min.js", "text/javascript");
    });

    server.serveStatic("/src/jquery-3.3.1.min.js", SD, "/web/src/jquery-3.3.1.min.js");    
    
    server.on("/src/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
                Serial.println(xPortGetCoreID());

        request->send(SD, "/web/src/bootstrap.bundle.min.js", "text/javascript");
    });
    server.serveStatic("/src/simple-datatables@latest.js", SD, "/web/src/simple-datatables@latest.js");
    server.serveStatic("/js/table.js", SD, "/web/js/table.js");

    server.on("/src/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
                Serial.println(xPortGetCoreID());

        request->send(SD, "/web/src/bootstrap.min.css", "text/css");
    });

    server.on("/timestamp", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getMainTimestamp(currentDateTime).c_str());
    });

    server.begin();
}


void secondCore( void * parameter)
{
    while(true)
    {
        // vTaskDelete(NULL);
        if(currentDateTime - measureSaveDateTime > RtcDateTime(config.timeBetweenSavingSamples/1000))
        {
            getSampleFunc();
        }
        if(currentDateTime - measureSaveDateTime > RtcDateTime(config.turnFanTime/1000))
        {
            if(digitalRead(FAN_PIN)!=HIGH)
            {
                Serial.println("Turning fan on");
                digitalWrite(FAN_PIN, HIGH);
            }
        }
        delay(1000);
    }
}