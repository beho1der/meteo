

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
#include <ESP8266WebServer.h>
#include <Hash.h>
#include <FS.h>

#include "config.h"
#include "BMP280.h"
#include "CO2.h"
#include "mqtt.h"
#include "OTA.h"
#include "LED.h"

//--------------------------------------------------WebServer
ESP8266WebServer server(80);
WiFiManager wifiManager;

float blink_CO2; 
//поправочный коэффициент температуры
float koeff = 0.78;

// кнопка для сброса настроек wifi
#define PIN_RESET_BUTTON 4        
int RESET = 0; 



// -------------------------------------------------------Время работы для прерывания loop
unsigned long times_1min =0;
unsigned long times_led =0;

void handleRoot();
void handleLogin();
void handleNotFound();
void send_sensors();

void setup() {
    Serial.begin(9600);
    // Инициализируем ленту.
    strip.begin();

    setLedColor(255,255,255);
    setLedColor(0,0,0);
    Serial.println("LED Ready"); 
    
    // доступ к настройкам по адресу http://192.168.4.1 
    wifiManager.autoConnect(CLIENT_ID);
    
    ota_init();
    Serial.println("OTA Ready");
    
    mqtt_init();
    Serial.println("MQTT Ready");
    
    
    bmp_init();
    Serial.println("BMP Ready");  

    server.on("/", HTTP_GET, handleRoot);
    server.onNotFound(handleNotFound); 
    server.on("/login", HTTP_POST, handleLogin);

}

void loop() {

 ArduinoOTA.handle();
 
    if (!client.connected()) {
    Serial.println("Attempting to reconnect MQTT...");
    mqtt_init();
  }

 //Обработка MQTT
 client.loop();
 
  // Считывание сенсоров один раз в минуту
if ((millis()- times_1min >READ_SENSORS_INTERVAL_MS)|| (times_1min>millis())){
    times_1min = millis();
    //Отправка показаний
    send_sensors();
}

  // Мигание светодиодом
if ((millis()- times_led >BLINK_INTERVAL_MS)|| (times_led>millis())){
    times_led = millis();
    //Отправка показаний
    blink_led(blink_CO2);
}

RESET = digitalRead(PIN_RESET_BUTTON);
    if( RESET == HIGH) {                                 
      Serial.println("Erase settings and restart ...");
      delay(1000);
      wifiManager.resetSettings();  
      ESP.restart();  
    }

server.handleClient();
delay(1000);

}

void handleRoot() {                     
  server.send(200, "text/html", "<form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'John Doe' and 'password123' ...</p>");
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void handleLogin() {                         // If a POST request is made to URI /login
  if( ! server.hasArg("username") || ! server.hasArg("password") 
      || server.arg("username") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  if(server.arg("username") == "John Doe" && server.arg("password") == "password123") { // If both the username and the password are correct
    server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
  } else {                                                                              // Username and password don't match
    server.send(401, "text/plain", "401: Unauthorized");
  }
}

void send_sensors(){
  
char result[8]; // Buffer big enough for 7-character float
float f = 0;

//----------------------------------------------------------KeepAlive

client.publish(ONLINE_TOPIC, "ON"); 

//-----------------------------------------------------------Температура

      f = bme.readTemperature()*koeff;
      dtostrf(f, 6, 2, result); // Leave room for too large numbers!
      client.publish(CURR_TEMP, result); 
      delay(2000);

//------------------------------------------------------------Влажность
      dtostrf(bme.readHumidity(), 6, 2, result); // Leave room for too large numbers!
      client.publish(HUMIDITY, result); 
      delay(2000);
          
//-------------------------------------------------------------Давление
      dtostrf(bme.readPressure()/133.32239F, 6, 2, result); // Leave room for too large numbers!
      client.publish(PRESSURE, result); 
      delay(2000);

//--------------------------------------------------------------CO2
f =-1;
f = read_sensor_mh_z19();
if (f== -1){
  Serial.println("CO2 error");

  for (int i=0; i<30; i++){
   f = read_sensor_mh_z19();
   if (f != -1){
    break; 
   }
   delay(2000);
  }
}
if (f!= -1){
      dtostrf(f, 6, 2, result); // Leave room for too large numbers!
      client.publish(CURR_CO2, result);

}
    blink_CO2 = f;

}
