#define OLED_SOFT_BUFFER_64 // использовать буфер на стороне микроконтроллера

#include <GyverOLED.h>
GyverOLED oled;

void setup() {
  oled.init(OLED128x64);
  oled.clear();
  oled.home();
  delay(1000);

  oled.scale1X();
  oled.print("Привет, ");
  oled.inverse(1);
  oled.println("мир!");
  oled.inverse(0);

  oled.scale2X();
  oled.print("Привет, ");
  oled.inverse(1);
  oled.println("мир!");

  oled.line(0, 0, 127, 63);
  oled.line(127, 32, 0, 63);
}

void loop() {

}
