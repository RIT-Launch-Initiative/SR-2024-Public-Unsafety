/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SD.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ICM20948.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11 
#define BME_CS 10

// For SPI mode, we need a CS pin
#define ICM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define ICM_SCK 13
#define ICM_MISO 12
#define ICM_MOSI 11

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

Adafruit_ICM20948 icm;

const int chipSelect = 4;

File dataFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!bme.begin()) {  
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1); //will pause program if bme doesnt work, probably dont do this
  }

  if (!icm.begin_I2C()) {
    // if (!icm.begin_SPI(ICM_CS)) {
//     if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {

    Serial.println("Failed to find ICM20948 chip");
    while (1) { //also will pause the program if it doesnt work lets not do that in the future
      delay(10);
    }
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  dataFile = SD.open("datalog.txt", FILE_WRITE);
}
//
////plan for 100Hz
////Data Format: CSV
//// time (ms), battery?, temp, pressure, ax, ay, az, rx, ry, rz, mx, my, mz
//
void loop() {

  long t = millis();
  int pressure = bme.readPressure();
  int temperature = bme.readTemperature();
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  icm.getEvent(&accel, &gyro, &temp, &mag);
  
  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.print(t); dataFile.print(',');
    dataFile.print(temperature); dataFile.print(',');
    //dataFile.print(pressure); dataFile.print(',');
    dataFile.print(accel.acceleration.x); dataFile.print(',');
    dataFile.print(accel.acceleration.y); dataFile.print(',');
    dataFile.print(accel.acceleration.z); dataFile.print(',');
    dataFile.print(gyro.gyro.x); dataFile.print(',');
    dataFile.print(gyro.gyro.y); dataFile.print(',');
    dataFile.print(gyro.gyro.z); dataFile.print(',');
    dataFile.print(mag.magnetic.x); dataFile.print(',');
    dataFile.print(mag.magnetic.y); dataFile.print(',');
    dataFile.print(mag.magnetic.z); dataFile.print(',');
    dataFile.flush();
  }
}
