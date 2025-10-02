#include "managers/SQLiteDb.hpp"

#include "utils/config.h"

int rows;

SQLiteDb::SQLiteDb(String localPath, String relativePath, String tableName)
{
    m_localPath = localPath;
    m_relativePath = relativePath;
    m_tableName = tableName;
    m_object = nullptr;
    m_mutex = xSemaphoreCreateMutex();
}

SQLiteDb::~SQLiteDb()
{
    if (m_mutex) {
        if (m_object) {
            sqlite3_close(m_object);
            m_object = nullptr;
        }
        vSemaphoreDelete(m_mutex);
        m_mutex = nullptr;
    }
}

void SQLiteDb::init()
{
    sqlite3_initialize();
}

void SQLiteDb::kill()
{
    sqlite3_shutdown();
}

int SQLiteDb::open()
{
    if (!xSemaphoreTake(m_mutex, portMAX_DELAY))
        return -1;

    strncpy(m_fileName, m_localPath.c_str(), sizeof(m_fileName));

    if (m_object) {
        int rc = sqlite3_close(m_object);
        if (rc != SQLITE_OK) {
            LOG_ERROR("Failed to close previous DB: " + String(rc));
        }
        m_object = nullptr;
    }

    int result = sqlite3_open(m_fileName, &m_object);
    if (result != SQLITE_OK || !m_object) {
        LOG_ERROR("Failed to open DB: " + String(result));
        m_object = nullptr;
    }

    xSemaphoreGive(m_mutex);
    return result;
}

void SQLiteDb::close()
{
    if (!xSemaphoreTake(m_mutex, portMAX_DELAY))
        return;

    if (m_object) {
        int rc = sqlite3_close(m_object);
        if (rc != SQLITE_OK) {
            LOG_ERROR("Failed to close DB: " + String(rc));
        }
        m_object = nullptr;
    }

    xSemaphoreGive(m_mutex);
}

void SQLiteDb::createTable()
{
    if (!xSemaphoreTake(m_mutex, portMAX_DELAY))
        return;

    if (!m_object) {
        LOG_DATABASE("No database open");
        xSemaphoreGive(m_mutex);
        return;
    }

    String sql = "CREATE TABLE IF NOT EXISTS " + m_tableName
                 + " (timestamp datetime NOT NULL PRIMARY KEY, Temperature float, Humidity FLOAT, "
                   "Pm10 FLOAT, Pm25 FLOAT, Pm100 float, Particles03 FLOAT, Particles05 FLOAT, "
                   "Particles10 FLOAT, Particles25 FLOAT, Particles50 FLOAT, Particles100 FLOAT);";

    char *errmsg = nullptr;
    int rc = sqlite3_exec(m_object, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        LOG_DATABASE("SQL error: " + String(sqlite3_extended_errcode(m_object)) + " "
                     + String(errmsg));
        sqlite3_free(errmsg);
    } else {
        LOG_DATABASE("Successfully created table: " + m_tableName + " or it already existed");
    }

    xSemaphoreGive(m_mutex);
}

int SQLiteDb::save(std::map<std::string, float> data,
                   float temperature,
                   float humidity,
                   String timestamp)
{
    if (!xSemaphoreTake(m_mutex, portMAX_DELAY))
        return -1;

    if (!m_object) {
        LOG_ERROR("Database does not exist. NULL");
        xSemaphoreGive(m_mutex);
        return -1;
    }

    String sql = "INSERT INTO " + m_tableName
                 + " ('Timestamp', 'Temperature', 'Humidity', 'Pm10', 'Pm25', 'Pm100', "
                   "'Particles03', 'Particles05', 'Particles10', 'Particles25', "
                   "'Particles50', 'Particles100') VALUES (?,?,?,?,?,?,?,?,?,?,?,?);";

    sqlite3_stmt *statement = nullptr;
    int rc = sqlite3_prepare_v2(m_object, sql.c_str(), -1, &statement, nullptr);

    if (rc != SQLITE_OK || !statement) {
        LOG_ERROR("Failed to prepare statement: " + String(sqlite3_errmsg(m_object)));
        xSemaphoreGive(m_mutex);
        return rc;
    }

    sqlite3_bind_text(statement, 1, timestamp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(statement, 2, temperature);
    sqlite3_bind_double(statement, 3, humidity);
    sqlite3_bind_double(statement, 4, data["pm10_standard"]);
    sqlite3_bind_double(statement, 5, data["pm25_standard"]);
    sqlite3_bind_double(statement, 6, data["pm100_standard"]);
    sqlite3_bind_double(statement, 7, data["particles_03um"]);
    sqlite3_bind_double(statement, 8, data["particles_05um"]);
    sqlite3_bind_double(statement, 9, data["particles_10um"]);
    sqlite3_bind_double(statement, 10, data["particles_25um"]);
    sqlite3_bind_double(statement, 11, data["particles_50um"]);
    sqlite3_bind_double(statement, 12, data["particles_100um"]);

    LOG_DATABASE("Executing: " + sql);

    rc = sqlite3_step(statement);

    if (rc != SQLITE_DONE) {
        LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(m_object)));
    } else {
        LOG_DATABASE("Successfully inserted record into table " + m_tableName);
    }

    sqlite3_finalize(statement);
    xSemaphoreGive(m_mutex);

    return rc;
}

static int selectCallback(void *data, int argc, char **argv, char **azColName)
{
    JsonArray *records = static_cast<JsonArray *>(data);
    StaticJsonDocument<600> doc;
    for (int i = 0; i < argc; i++)
        doc[azColName[i]] = argv[i];
    records->add(doc);
    return 0;
}

int SQLiteDb::select(String datetime, JsonArray *array)
{
    if (!xSemaphoreTake(m_mutex, portMAX_DELAY))
        return -1;

    if (!m_object) {
        LOG_ERROR("Database does not exist. NULL");
        xSemaphoreGive(m_mutex);
        return -1;
    }

    String sql;
    if (datetime != "null")
        sql = "SELECT * FROM " + m_tableName + " WHERE timestamp > '" + datetime.c_str()
              + "' LIMIT 50;";
    else
        sql = "SELECT * FROM " + m_tableName + " ORDER BY timestamp LIMIT 50;";

    LOG_DATABASE("Executing: " + sql);

    int rc;
    rc = sqlite3_exec(m_object, sql.c_str(), selectCallback, array, &m_errorMessage);

    if (rc != SQLITE_OK) {
        LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(m_object)) + " "
                  + String(m_errorMessage));
        sqlite3_free(m_errorMessage);
        rc = 1;
    }

    xSemaphoreGive(m_mutex);
    return rc;
}

int SQLiteDb::getLastRecord(JsonArray *array)
{
    if (!xSemaphoreTake(m_mutex, portMAX_DELAY))
        return -1;

    if (!m_object) {
        LOG_ERROR("Database does not exist. NULL");
        xSemaphoreGive(m_mutex);
        return -1;
    }

    String sql = "SELECT * FROM " + m_tableName + " ORDER BY timestamp DESC LIMIT 1";
    LOG_DATABASE("Executing: " + sql);

    int rc;
    rc = sqlite3_exec(m_object, sql.c_str(), selectCallback, array, &m_errorMessage);

    if (rc != SQLITE_OK) {
        LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(m_object)) + " "
                  + String(m_errorMessage));
        sqlite3_free(m_errorMessage);
    } else {
        LOG_DATABASE("Successfully fetched last record from table " + m_tableName);
    }

    xSemaphoreGive(m_mutex);
    return rc;
}

String SQLiteDb::getLocalPath()
{
    return m_localPath;
}

String SQLiteDb::getRelativePath()
{
    return m_relativePath;
}
