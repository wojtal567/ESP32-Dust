#include <Arduino.h>
#include <SPI.h>
#include <RTClib.h>

#define FAN_PIN 4

RTC_DS1307 rtc;

DateTime lastSampleTime, turnFanOnTime;

void setup() {
  // PMS things
  pinMode(FAN_PIN, OUTPUT);

  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  // com
  Serial.begin(115200);

  // rtc
  if (!rtc.begin()) {
    Serial.println("RTC NOT FOUND");
  }

  if (!rtc.isrunning()) {
    Serial.println(
        "RTC is not running, need to set the time(for now compilation time)");

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  lastSampleTime = rtc.now();
  turnFanOnTime = rtc.now();
  digitalWrite(FAN_PIN, HIGH);
}

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um,
      particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;

boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }

  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }

  uint8_t buffer[32];
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // get checksum ready
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }

  /* debugging
for (uint8_t i=2; i<32; i++) {
  Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
}
Serial.println();
*/

  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}

void printPMSData() {
  Serial.println();
  Serial.println("---------------------------------------");
  Serial.println("Concentration Units (standard)");
  Serial.print("PM 1.0: ");
  Serial.print(data.pm10_standard);
  Serial.print("\t\tPM 2.5: ");
  Serial.print(data.pm25_standard);
  Serial.print("\t\tPM 10: ");
  Serial.println(data.pm100_standard);
  Serial.println("---------------------------------------");
  Serial.println("Concentration Units (environmental)");
  Serial.print("PM 1.0: ");
  Serial.print(data.pm10_env);
  Serial.print("\t\tPM 2.5: ");
  Serial.print(data.pm25_env);
  Serial.print("\t\tPM 10: ");
  Serial.println(data.pm100_env);
  Serial.println("---------------------------------------");
  Serial.print("Particles > 0.3um / 0.1L air:");
  Serial.println(data.particles_03um);
  Serial.print("Particles > 0.5um / 0.1L air:");
  Serial.println(data.particles_05um);
  Serial.print("Particles > 1.0um / 0.1L air:");
  Serial.println(data.particles_10um);
  Serial.print("Particles > 2.5um / 0.1L air:");
  Serial.println(data.particles_25um);
  Serial.print("Particles > 5.0um / 0.1L air:");
  Serial.println(data.particles_50um);
  Serial.print("Particles > 10.0 um / 0.1L air:");
  Serial.println(data.particles_100um);
  Serial.println("---------------------------------------");
}

void loop() {
  if ((rtc.now() - turnFanOnTime).minutes() >= 1 &&
      digitalRead(FAN_PIN) == HIGH) {
    Serial.println("Trying to take sample...");

    if (readPMSdata(&Serial2)) {
      DateTime now = rtc.now();
      String dateForPrint = now.hour() + (String) ":" + now.minute() +
                            (String) ":" + now.second() + ", " + now.day() +
                            "." + now.month() + "." + now.year();
      Serial.print("\n");
      Serial.println(dateForPrint);

      printPMSData();

      Serial.println("Turning fan off");
      digitalWrite(FAN_PIN, LOW);
      lastSampleTime = rtc.now();
    }
  }

  if ((rtc.now() - lastSampleTime).minutes() >= 5 &&
      digitalRead(FAN_PIN) == LOW) {
    Serial.println("Turning on the fan...");
    digitalWrite(FAN_PIN, HIGH);
    turnFanOnTime = rtc.now();
  }

  delay(500);
}
