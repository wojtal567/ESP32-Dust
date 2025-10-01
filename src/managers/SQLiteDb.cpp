#include "managers/SQLiteDb.hpp"

#include "utils/config.h"

int rows;

SQLiteDb::SQLiteDb(String localPath, String relativePath, String tableName)
{
    _localPath = localPath;
    _relativePath = relativePath;
    _tableName = tableName;
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
    strncpy(fileName, _localPath.c_str(), 100);
    if (object != NULL)
        sqlite3_close(object);
    return sqlite3_open(fileName, &object);
}

void SQLiteDb::close()
{
    sqlite3_close(object);
}

void SQLiteDb::createTable()
{
    if (object == NULL) {
        LOG_DATABASE("No database open");
    }

    String sql = "CREATE table if not exists " + _tableName
                 + " (timestamp datetime NOT NULL PRIMARY KEY, Temperature float, Humidity FLOAT, "
                   "Pm10 FLOAT, Pm25 FLOAT, Pm100 float, Particles03 FLOAT, Particles05 FLOAT, "
                   "Particles10 FLOAT, Particles25 FLOAT, Particles50 FLOAT, Particles100 FLOAT)";
    int rc = sqlite3_exec(object, sql.c_str(), 0, (void *)"Output:", &zErrorMessage);

    if (rc != SQLITE_OK) {
        LOG_DATABASE("SQL error: " + String(sqlite3_extended_errcode(object)) + " "
                     + String(zErrorMessage));
    } else if (rc == SQLITE_OK) {
        LOG_DATABASE("Successfully created table: " + _tableName + " or it already exists");
    }
}

int SQLiteDb::save(std::map<std::string, float> data,
                   float temperature,
                   float humidity,
                   String timestamp)
{
    if (object == NULL) {
        LOG_ERROR("Database does not exist. NULL");
        return 0;
    }

    String sql = "INSERT INTO " + _tableName
                 + " ('Timestamp', 'Temperature', 'Humidity', 'Pm10', 'Pm25', 'Pm100', "
                   "'Particles03', 'Particles05', 'Particles10', 'Particles25', "
                   "'Particles50', 'Particles100') VALUES (?,?,?,?,?,?,?,?,?,?,?,?);";

    sqlite3_stmt *statement;
    int rc = sqlite3_prepare_v2(object, sql.c_str(), -1, &statement, 0);

    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to prepare statement: " + String(sqlite3_errmsg(object)));
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
        LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(object)));
    } else if (rc == SQLITE_OK) {
        LOG_DATABASE("Successfully inserted record into table " + _tableName);
    }

    sqlite3_finalize(statement);
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
    if (object == NULL) {
        LOG_ERROR("Database does not exist. NULL");
        return 0;
    }
    String sql = "";
    if (datetime != "null")
        sql = "select * from " + _tableName + " where timestamp > '" + datetime.c_str()
              + "' limit 50;";
    else
        sql = "select * from " + _tableName + " order by timestamp limit 50;";
    LOG_DATABASE("Executing: " + sql);
    int rc = sqlite3_exec(object, sql.c_str(), selectCallback, array, &zErrorMessage);

    if (rc != SQLITE_OK) {
        LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(object)) + " "
                  + String(zErrorMessage));
        sqlite3_free(zErrorMessage);
        return 1;
    } else
        LOG_DATABASE(zErrorMessage);

    return rc;
}

int SQLiteDb::getLastRecord(JsonArray *array)
{
    if (object == NULL) {
        LOG_ERROR("Database does not exist. NULL");
        return 0;
    }

    String sql = "SELECT * FROM " + _tableName + " order by timestamp desc limit 1";
    LOG_DATABASE("Executing: " + sql);
    int rc = sqlite3_exec(object, sql.c_str(), selectCallback, array, &zErrorMessage);
    if (rc != SQLITE_OK) {
        LOG_ERROR("SQL error: " + String(sqlite3_extended_errcode(object)) + " "
                  + String(zErrorMessage));
        sqlite3_free(zErrorMessage);
    } else
        LOG_DATABASE("Successfully fetched last record from table " + _tableName);

    return rc;
}

String SQLiteDb::getLocalPath()
{
    return _localPath;
}

String SQLiteDb::getRelativePath()
{
    return _relativePath;
}
