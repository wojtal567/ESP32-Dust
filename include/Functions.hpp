#include <GlobalVariables.hpp>

//Get single sample and set text
void getSampleFunc()
{
    sht30.get();
    if (pmsSensor->readData())
    {
        Serial.println("Succesfully read data from dust sensor.");
        std::map<std::string, float> tmpData = pmsSensor->returnData();
        pmsSensor->dumpSamples();
        data = tmpData;
        config.currentSampleNumber++;
        temp = sht30.cTemp;
        humi = sht30.humidity;
        Serial.print("Temperature: "); Serial.print(temp);
        Serial.print("\nHumidity: "); Serial.print(humi);
        Serial.println();
        if(Rtc.GetIsRunning())
        {
            lastSampleTimestamp = getMainTimestamp(Rtc);
            Serial.print("lastSampleTimestamp przed wrzuceniem do bazy: " + lastSampleTimestamp);
            mySDCard.save(data, temp, humi, lastSampleTimestamp, &sampleDB, &Serial);
        }
        else
            Serial.println("RTC is not running, not saving");
    
        digitalWrite(FAN_PIN, LOW);
        measureSaveDateTime = Rtc.GetDateTime();
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