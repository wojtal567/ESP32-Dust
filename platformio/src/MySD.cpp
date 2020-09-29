#include <MySD.hpp>

MySD::MySD(int port)
{
    _port = port;
}

bool MySD::begin()
{
    return SD.begin(_port);
}

void MySD::end()
{
    SD.end();
}

bool MySD::start(SQLiteDb *object, Stream *debugger)
{
    bool result = begin();

    if(result)
    {
        object->init();
        object->open();
        object->createTable(debugger);
        object->close();
        object->kill();
    }
    return result;
}

void MySD::save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, SQLiteDb *object, Stream *debugger)
{
  debugger->println("MySD::save");
    if(begin())
    {
        debugger->println("SD Card detected");
        if(SD.exists(object->getRelativePath()))
        {
            debugger->println("Database " + object->getLocalPath() + " exists. Saving data...");
            object->init();
            object->open();
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
        else
        {
          debugger->println("Database " + object->getLocalPath() + " don't exist. Saving data...");
            object->init();
            object->open();
            object->createTable(debugger);
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
    }

}

void MySD::select(SQLiteDb *object, Stream *debugger, String datetime, JsonArray* array)
{
    debugger->println("MySD::select");
    if(begin())
    {
        debugger->println("SD Card detected");
        if(SD.exists(object->getRelativePath()))
        {
            debugger->println("Database " + object->getLocalPath() + " exists.");
            object->init();
            object->open();
            object->select(debugger, datetime, array);
            object->close();
            object->kill();
        }
    }
}

void MySD::getLastRecord(SQLiteDb *object, Stream *debugger, JsonArray* array)
{
    debugger->println("MySD::getLastRecord");
    if(begin())
    {
        debugger->println("SD Card detected");
        if(SD.exists(object->getRelativePath()))
        {
            debugger->println("Database " + object->getLocalPath() + " exists.");
            object->init();
            object->open();
            object->getLastRecord(debugger, array);
            object->close();
            object->kill();
        }
    }
}