#include <Adafruit_ICM20948.h>
#include <Adafruit_BME280.h>
#include <SD.h>

#define cardSelect 4
#define VBATPIN A7

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11 
#define BME_CS 10

Adafruit_ICM20948 icm;
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

File dataFile;

void setup() {
  // Open serial communications and wait for port to open:
//  Serial.begin(115200);
//  while(!Serial);
//  Serial.println("test");

  if (!icm.begin_I2C()) {
//    Serial.println("ICM not found");
  }

  if (!bme.begin()) {
//    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  if (!SD.begin(cardSelect)) {
//    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  dataFile = SD.open("datalog.txt", FILE_WRITE);
}

void loop() {
    long t = millis();
    int pressure = bme.readPressure();
    int temperature = bme.readTemperature();
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag;
    sensors_event_t temp;
    icm.getEvent(&accel, &gyro, &temp, &mag);
  
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    dataFile.print(t);
    dataFile.print(",");
    dataFile.print(accel.acceleration.x);
    dataFile.print(",");
    dataFile.print(accel.acceleration.y);
    dataFile.print(",");
    dataFile.print(accel.acceleration.z);
    dataFile.print(",");
    dataFile.print(gyro.gyro.x);
    dataFile.print(",");
    dataFile.print(gyro.gyro.y);
    dataFile.print(",");
    dataFile.print(gyro.gyro.z);
    dataFile.print(",");
    dataFile.print(mag.magnetic.x);
    dataFile.print(",");
    dataFile.print(mag.magnetic.y);
    dataFile.print(",");
    dataFile.print(mag.magnetic.z);
    dataFile.print(pressure);
    dataFile.print(",");
    dataFile.print(temperature);
    dataFile.print(",");
    dataFile.print(measuredvbat);
    dataFile.println();
    dataFile.flush();
    delay(100);
}
