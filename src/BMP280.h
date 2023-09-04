#include <Adafruit_BME280.h>

#define BME280_ADDRESS 0x76

Adafruit_BME280 bme;

void bmp_init(){
  if (!bme.begin(BME280_ADDRESS)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}
