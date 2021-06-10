#include <MySD.hpp>
#include <RtcDS1307.h>
#include <Wire.h>
#include <PMS5003.hpp>
#include <WEMOS_SHT3X.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
// #include <WebServer.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>

#define FAN_PIN 4        // * number of PIN which controls the PMS fan
#define MY_SD_CARD_PIN 27 // * pin of SD_CS



AsyncWebServer server(80);
AsyncWebSocket socket("/ws");


// ! CONFIG ============================================|
std::string configFilePath = "/settings.json";

RtcDateTime measureSaveDateTime;
RtcDateTime currentDateTime;


Config config =
    {
        "",
        "",
        60000,
        3600000,
        30000,
        5,
        0,
        30000};

int ntpTimeOffset = 7200; //poland, winter - 3600, summer (DST) - 7200 

//RTC, PMS5003 and SHT30 objects declaration
RtcDS1307<TwoWire> Rtc(Wire);
PMS5003 *pmsSensor;
SHT3X sht30(0x45);

std::map<std::string, float> data;
// const char *labels[15] = {
//     "framelen",
//     "pm10_standard",
//     "pm25_standard",
//     "pm100_standard",
//     "pm10_env",
//     "pm25_env",
//     "pm100_env",
//     "particles_03um",
//     "particles_05um",
//     "particles_10um",
//     "particles_25um",
//     "particles_50um",
//     "particles_100um",
//     "unused",
//     "checksum"};

//NTPClient declarations
static const char ntpServerName[] = "europe.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient dateTimeClient(ntpUDP, ntpServerName, ntpTimeOffset);

//SD Card and sqlite database objects declaration
MySD mySDCard(MY_SD_CARD_PIN);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");

String lastSampleTimestamp;

//Temperature, relative humidity and pm2.5 per ug/m3 variables
float temp, humi;