#include <LVGLFunctions.hpp>
#include "GlobalVariables.hpp"

void inactive_screen(lv_task_t *task)
{
    if (config.lcdLockTime != -1)
    {
        if (lv_disp_get_inactive_time(NULL) > config.lcdLockTime)
        {
            if (lv_scr_act() != lockScr)
                lv_disp_load_scr(lockScr);
        }
    }
}

void fetchLastRecordAndSynchronize(lv_task_t *task)
{

    if (WiFi.status() == WL_CONNECTED && appIpAddress != "")
    {
        Serial.println("Starting synchronizing process...");
        HTTPClient getHttp;
        String url = "http://" + appIpAddress + "/fetch/last";
        Serial.print(url);
        if (getHttp.begin(url.c_str()))
        {
            Serial.print(getHttp.getString());
            uint8_t responseCode = getHttp.GET();

            if (responseCode == 200)
            {
                Serial.println("GET /last succesful.");
                Serial.println("HTTP RESPONSE CODE: " + (String)responseCode);
                StaticJsonDocument<600> response, doc1;

                DeserializationError err = deserializeJson(response, getHttp.getString());
                Serial.println("Deserialization error: " + (String)err.c_str());
                JsonArray lastRecord = doc1.to<JsonArray>();

                mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecord);
                DynamicJsonDocument doc(33000);
                if ((response[0]["timestamp"].as<String>() != lastRecord[0]["timestamp"].as<String>()) || (response[0]["timestamp"].as<String>() == "null"))
                {
                    Serial.println("Got last record that looks good. Parsing and sending data to Server App...");
                    JsonArray records = doc.to<JsonArray>();
                    mySDCard.select(&sampleDB, &Serial, response[0]["timestamp"].as<String>(), &records);
                    String json = "";

                    serializeJson(doc, json);
                    getHttp.begin("http://" + appIpAddress + "/submit");
                    getHttp.addHeader("Content-Type", "application/json");
                    getHttp.POST(json);
                    Serial.print("POST RESPONSE:" + getHttp.getString());
                    getHttp.end();
                    //lv_task_ready(getAppLastRecordAndSynchronize);
                }
            }
            else
            {
                Serial.println("ERROR FETCHING DATA. CODE: " + (String)responseCode);
                lv_task_set_prio(getAppLastRecordAndSynchronize, LV_TASK_PRIO_OFF);
            }
        }
        else
        {
            Serial.print("Wrong url");
            lv_task_set_prio(getAppLastRecordAndSynchronize, LV_TASK_PRIO_OFF);
        }
        getHttp.end();
    }
}

void dateTimeFunc(lv_task_t *task)
{

    if (rtcManager.isRunning()) {
        mainScreen->updateDateTimeLabel(
            Utils::formatMainTimestamp(rtcManager.getCurrentDateTime()).c_str());
        lv_label_set_text(labelTimeLock, rtcManager.getTime().c_str());
        lv_label_set_text(labelDateLock, rtcManager.getDate().c_str());
        if (inTimeSettings == false)
        {
            lv_spinbox_set_value(timeHour,
                                 rtcManager.getTime()
                                     .substring(0, rtcManager.getTime().indexOf(":"))
                                     .toInt());
            lv_spinbox_set_value(timeMinute, rtcManager.getTime().substring(3, 5).toInt());
            lv_label_set_text(dateBtnLabel, rtcManager.getDate().c_str());
        }
    } else {
        if (inTimeSettings == false)
            lv_label_set_text(dateBtnLabel, "01.01.2021");

        mainScreen->updateDateTimeLabel("\0");
    }
}

void statusFunc(lv_task_t *task)
{
    const bool isNetworkConnected = networkManager.isConnected();
    mainScreen->updateWiFiStatus(isNetworkConnected);
    if (isNetworkConnected) {
        lv_obj_set_hidden(wifiStatusAtLockWarning, true);
        lv_label_set_text(infoWifiAddressLabel, networkManager.getIpAddress().c_str());
    } else {
        lv_obj_set_hidden(wifiStatusAtLockWarning, false);
        lv_label_set_text(infoWifiAddressLabel, "No WiFi connection");
    }

    const bool isSDCardConnected = mySDCard.start(&sampleDB, &Serial2);

    if (isSDCardConnected) {
        lv_obj_set_hidden(sdStatusAtLockWarning, true);
        if (!networkManager.isConnected() && (config.ssid != "" && config.password != "")) {
            networkManager.setCredentials(config.ssid.c_str(), config.password.c_str());
            networkManager.connect();
        }
    } else {
        lv_obj_set_hidden(sdStatusAtLockWarning, false);
    }

    mainScreen->updateSDStatus(isSDCardConnected);
}