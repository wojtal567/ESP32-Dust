#pragma once

#include <ArduinoJson.h>
#include <Stream.h>
#include <sqlite3.h>
#include <stddef.h>
#include <cstring>
#include <map>
#include <string>

class SQLiteDb
{
public:
    SQLiteDb(String localPath, String relativePath, String tableName);
    ~SQLiteDb();
    void init();
    void kill();
    int open();
    void close();
    void createTable();
    int save(std::map<std::string, float> data, float temperature, float humidity, String timestamp);
    int select(String datetime, JsonArray *array);
    int getLastRecord(JsonArray *array);
    String getLocalPath();
    String getRelativePath();

private:
    sqlite3 *m_object;
    char m_fileName[100] = "\0";
    String m_localPath;
    String m_relativePath;
    String m_tableName;
    char *m_errorMessage = 0;
    SemaphoreHandle_t m_mutex;
};
