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
    if (xSemaphoreTake(m_mutex, portMAX_DELAY)) {
        strncpy(m_fileName, m_localPath.c_str(), 100);
        if (m_object != NULL) {
            sqlite3_close(m_object);
            m_object = nullptr;
        }

        auto result = sqlite3_open(m_fileName, &m_object);
        xSemaphoreGive(m_mutex);
        return result;
    }
    return -1;
}

void SQLiteDb::close()
{
    if (xSemaphoreTake(m_mutex, portMAX_DELAY)) {
        sqlite3_close(m_object);

        xSemaphoreGive(m_mutex);
    }
}

void SQLiteDb::createTable()
{
    if (m_object == NULL) {
        LOG_DATABASE("No database open");
    }

    String sql = "CREATE table if not exists " + m_tableName
                 + " (timestamp datetime NOT NULL PRIMARY KEY, Temperature float, Humidity FLOAT, "
                   "Pm10 FLOAT, Pm25 FLOAT, Pm100 float, Particles03 FLOAT, Particles05 FLOAT, "
                   "Particles10 FLOAT, Particles25 FLOAT, Particles50 FLOAT, Particles100 FLOAT)";
    if (xSemaphoreTake(m_mutex, portMAX_DELAY)) {

        int rc = sqlite3_exec(m_object, sql.c_str(), 0, (void *)"Output:", &m_errorMessage);

        if (rc != SQLITE_OK) {
            LOG_DATABASE("SQL error: " + String(sqlite3_extended_errcode(m_object)) + " "
                         + String(m_errorMessage));
        } else if (rc == SQLITE_OK) {
            LOG_DATABASE("Successfully created table: " + m_tableName + " or it already exists");
        }
        xSemaphoreGive(m_mutex);
    }
}

int SQLiteDb::save(std::map<std::string, float> data,
                   float temperature,
                   float humidity,
                   String timestamp)
{
    if (m_object == NULL) {
        LOG_ERROR("Database does not exist. NULL");
        return 0;
    }

    String sql = "INSERT INTO " + m_tableName
                 + " ('Timestamp', 'Temperature', 'Humidity', 'Pm10', 'Pm25', 'Pm100', "
                   "'Particles03', 'Particles05', 'Particles10', 'Particles25', "
                   "'Particles50', 'Particles100') VALUES (?,?,?,?,?,?,?,?,?,?,?,?);";

    int rc;
    if (xSemaphoreTake(m_mutex, portMAX_DELAY)) {

        sqlite3_stmt *statement;
        rc = sqlite3_prepare_v2(m_object, sql.c_str(), -1, &statement, 0);

        if (rc != SQLITE_OK) {
            LOG_ERROR("Failed to prepare statement: " + String(sqlite3_errmsg(m_object)));
        } else {

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
            } else if (rc == SQLITE_OK) {
                LOG_DATABASE("Successfully inserted record into table " + m_tableName);
            }

            sqlite3_finalize(statement);
        }
        xSemaphoreGive(m_mutex);
    }
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
    if (m_object == NULL) {
        LOG_ERROR("Database does not exist. NULL");
        return 0;
    }
    String sql = "";
    if (datetime != "null")
        sql = "select * from " + m_tableName + " where timestamp > '" + datetime.c_str()
              + "' limit 50;";
    else
        sql = "select * from " + m_tableName + " order by timestamp limit 50;";
    LOG_DATABASE("Executing: " + sql);
    int rc;
    if (xSemaphoreTake(m_mutex, portMAX_DELAY)) {

        rc = sqlite3_exec(m_object, sql.c_str(), selectCallback, array, &m_errorMessage);

        if (rc != SQLITE_OK) {
            LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(m_object)) + " "
                      + String(m_errorMessage));
            sqlite3_free(m_errorMessage);
            rc = 1;
        } else
            LOG_DATABASE(m_errorMessage);
        xSemaphoreGive(m_mutex);
    }

    return rc;
}

int SQLiteDb::getLastRecord(JsonArray *array)
{
    if (m_object == NULL) {
        LOG_ERROR("Database does not exist. NULL");
        return 0;
    }

    String sql = "SELECT * FROM " + m_tableName + " order by timestamp desc limit 1";
    LOG_DATABASE("Executing: " + sql);
    int rc;
    if (xSemaphoreTake(m_mutex, portMAX_DELAY)) {
        rc = sqlite3_exec(m_object, sql.c_str(), selectCallback, array, &m_errorMessage);
        if (rc != SQLITE_OK) {
            LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(m_object)) + " "
                      + String(m_errorMessage));
            sqlite3_free(m_errorMessage);
        } else
            LOG_DATABASE("Successfully fetched last record from table " + m_tableName);

        xSemaphoreGive(m_mutex);
    }

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
