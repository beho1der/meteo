#include "Adafruit_NeoPixel.h"

// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 2
 
// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 14
// Создаем переменную strip для управления нашей лентой.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setLedColor(int r,int g, int b){
 // Включаем все светодиоды.
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(r, g, b)); // Красный цвет.
  }
  // Передаем цвета ленте.
  strip.show();
  // Ждем 500 мс.
  delay(500);
}

void blink_led(float f){
  // Включаем все светодиоды.
    if (f>1400){
    setLedColor(255,0,0); // Красный цвет.
    }
    else if (f>1000 & f<1400){
    setLedColor(255,104,0); // Оранжевый цвет.      
    }
    else if (f>800 & f<1000){
    setLedColor(255,239,0); // желтый цвет.      
    }
    else if (f<800){
    setLedColor(0,255,0); // Зеленый цвет. 
    //delay(1000);
    //setLedColor(0,0,0); // Гасим светодиоды        
    }
    else if (f == -1){
    setLedColor(190, 3, 253); // Фиолетовый цвет.
    //delay(1000);
    //setLedColor(0,0,0); // Гасим светодиоды  
    }

}
