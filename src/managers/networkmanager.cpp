#include "managers/networkmanager.h"

#include "managers/taskmanager.h"
#include "utils/constants.h"
#include "utils/stringConstants.h"

NetworkManager::NetworkManager(MySD &sdCard)
    : m_sdCard(sdCard)
    , m_taskManager(nullptr)
    , m_server(80)
    , m_wifiTaskHandle(nullptr)
{}

bool NetworkManager::connect()
{
    WiFi.begin(m_ssid.c_str(), m_password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
}

bool NetworkManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

String NetworkManager::getIpAddress() const
{
    if (isConnected())
        return WiFi.localIP().toString();
    return "";
}

void NetworkManager::setCredentials(const String &ssid, const String &password)
{
    m_ssid = ssid;
    m_password = password;
}

void NetworkManager::connectAsync(const String &ssid, const String &password)
{
    // Store credentials
    m_ssid = ssid;
    m_password = password;

    // If already connecting, stop the previous task
    if (m_wifiTaskHandle != nullptr) {
        vTaskDelete(m_wifiTaskHandle);
        m_wifiTaskHandle = nullptr;
    }

    // Create FreeRTOS task for WiFi connection
    xTaskCreate(wifiConnectionTask, "WiFiConnection", 4096, this, 1, &m_wifiTaskHandle);
}

// Static task function that runs in separate thread
void NetworkManager::wifiConnectionTask(void *parameter)
{
    NetworkManager *networkManager = static_cast<NetworkManager *>(parameter);

    // Start WiFi connection
    WiFi.begin(networkManager->m_ssid.c_str(), networkManager->m_password.c_str());

    // Wait for connection with timeout
    int attempts = 0;
    const int maxAttempts = 20; // 10 seconds timeout (500ms * 20)

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        vTaskDelay(500 / portTICK_PERIOD_MS); // Non-blocking delay
        attempts++;
    }

    // Clean up task handle
    networkManager->m_wifiTaskHandle = nullptr;

    // Delete this task
    vTaskDelete(NULL);
}

WebServer &NetworkManager::getServer()
{
    return m_server;
}

void NetworkManager::setupServer()
{
    // Setup API routes
    m_server.on("/", HTTP_GET, [this]() {
        m_server.send(200, F("text/html"), F("You have entered the wrong neighbourhood"));
    });
    m_server.on(F("/setAppIp"), HTTP_POST, [this]() { this->handleSetAppIp(); });

    // Setup 404 handler
    m_server.onNotFound([this]() { this->handleNotFound(); });

    m_server.begin();
    Serial.print("[" + String(millis()) + "] ");
    Serial.println("HTTP server started on port 80");
}

void NetworkManager::handleServerClient()
{
    m_server.handleClient();
}

void NetworkManager::setTaskManager(TaskManager *taskManager)
{
    m_taskManager = taskManager;
}

void NetworkManager::handleSetAppIp()
{
    String postBody = m_server.arg("plain");
    Serial.print("[" + String(millis()) + "] ");
    Serial.println(postBody);
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        Serial.print("[" + String(millis()) + "] ");
        Serial.print(F(error.c_str()));

        m_server.send(400,
                      F("text/html"),
                      "Error while parsing json body! <br>" + (String)error.c_str());
    } else {
        JsonObject postObj = doc.as<JsonObject>();

        Serial.print("[" + String(millis()) + "] ");
        Serial.print(F("HTTP Method: "));
        Serial.print("[" + String(millis()) + "] ");
        Serial.println(m_server.method());

        if (m_server.method() == HTTP_POST) {
            if (postObj.containsKey("ip")) {
                String appIpAddress = postObj["ip"].as<String>();

                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                String buf;
                serializeJson(doc, buf);

                if (m_taskManager) {
                    m_taskManager->updateGetAppLastRecordAndSynchronizeTaskPrio(LV_TASK_PRIO_MID);
                    m_taskManager->setAppIpAddress(appIpAddress);
                }

                m_server.send(201, F("application/json"), buf);
            } else {
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                doc["message"] = F("No data found or incorrect!");

                String buf;
                serializeJson(doc, buf);

                m_server.send(400, F("application/json"), buf);
            }
        }
    }
}

void NetworkManager::handleNotFound()
{
    String message = "File Not Found \n\n" + (String) "URI: " + m_server.uri()
                             + "\n Method: " + (m_server.method() == HTTP_GET)
                         ? "GET"
                         : "POST" + (String) "\n Arguments: " + m_server.args() + "\n";

    for (uint8_t i = 0; i < m_server.args(); i++)
        message += " " + m_server.argName(i) + ": " + m_server.arg(i) + "\n";

    m_server.send(404, "text/plain", message);
}
