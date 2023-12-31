
unsigned char response[9];
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  byte zerocalib[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
  byte spancalib[9] = {0xFF, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00,0x77};
  byte autocalib_on[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};
  byte autocalib_off[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};

int rst = 0;

float read_sensor_mh_z19() {
//co2 MH-Z19
  Serial.write(cmd, 9);
  memset(response, 0, 9);
  Serial.readBytes(response, 9);
  int i;
  byte crc = 0;
  for (i = 1; i < 8; i++) crc+=response[i];
  crc = 255 - crc;
  crc++;

  if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
   // Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));
        while (Serial.available()) {
        Serial.read();        
        }
    return -1;
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    float return_CO2 = (256*responseHigh) + responseLow;
   // Serial.println(return_CO2);
    return return_CO2;
  }

}


void ZERO_POINT_CALIBRATION(){ 
    Serial.write(zerocalib, 9); // Команда модулю
    memset(response, 0, 9);          //Обнуление буфера
}

void abc_off(){
 
    Serial.write(autocalib_off, 9); // Команда модулю
    memset(response, 0, 9);          //Обнуление буфера 
}

void abc_on(){
 
    Serial.write(autocalib_on, 9); // Команда модулю
    memset(response, 0, 9);          //Обнуление буфера 
}

