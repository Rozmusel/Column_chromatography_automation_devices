#include <GyverOLED.h>          // Библиотека дисплея
#include <EncButton2.h>         // Библиотека кнопок
#include <EEPROM.h>             // Библиотека управления EEPROM
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Arduino.h"

#define ITEMS 5
#define ITEMS2 3
#define ITEMS3 3
#define ITEMS4 3
#define ITEMS5 5

#define EB_DEB 50       // дебаунс кнопки, мс
#define EB_CLICK 50// таймаут накликивания кнопки, 

#define L_PHOTORESISTOR_TRESHOLD 700
#define R_PHOTORESISTOR_TRESHOLD 400
#define U_PHOTORESISTOR_TRESHOLD 600
#define HOLL_TRESHOLD 420

#define SPEED 3000

#define BIG_LASER 3
#define SMALL_LASERS 4
#define PIN_DIR 5
#define PIN_STEP 6
#define SWITCH_OFF 8
#define R_PHOTORESISTOR A2
#define L_PHOTORESISTOR A1
#define U_PHOTORESISTOR A0
#define HOLL_SENSOR A3

DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(2, 7); // RX, TX


EncButton2<EB_BTN> btn[4];      // инициализация кнопок

EncButton2<EB_BTN> back(INPUT_PULLUP, 11);
EncButton2<EB_BTN> ok(INPUT_PULLUP, 10);
EncButton2<EB_BTN> down(INPUT_PULLUP, 9);
EncButton2<EB_BTN> up(INPUT_PULLUP, 12);

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

void Launch();                  // Функция запуска сортировки
void Settings();                // Функция перехода в настройки
void BetterCallMe();            // Функция перехода в обратную связь
void Calibration();             // Функция перехода в раздел калибровки колб
void Sound();                   // Функция перехода в раздел изменения звука
void F25ml();                   // Функция перехода в раздел калибровки колбы 25 мл
void F50ml();                   // Функция перехода в раздел калибровки колбы 50 мл
void Logo();                    // Функция запуска анимации
void Mods();                    // Функция перехода в раздел с дополнениями
void Loud();                    // Функция перехода в раздел изменения громкости
void Succes();
void WatchFlask();
void df_step();
void Waiter();
void Base();
void Amongus();
void PoleChydes();



const uint8_t bitmap_127x27[] PROGMEM = { //Битмап логотипа без буквы
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x04, 0x84, 0x68, 0x18, 0x10, 0x30, 0x20, 0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60, 0x18, 0x0E, 0x03, 0x02, 0x02, 0x04, 0x04, 0x08, 0x18, 0x13, 0x32, 0x24, 0x44, 0xC8, 0x98, 0x90, 0x30, 0x20, 0x40, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x1C, 0x06, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x10, 0x10, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x06, 0x1C, 0x30, 0xC0, 0x80, 0x00, 0x00, 0xC0, 0x40, 0x20, 0x20, 0x90, 0x90, 0xC8, 0x4C, 0x24, 0x26, 0x12, 0x10, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0C, 0x38, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x30, 0x0C, 0x0F, 0x19, 0x70, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC1, 0x31, 0x1E, 0x86, 0x60, 0x30, 0x0C, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60, 0x18, 0x86, 0xE3, 0x30, 0x0C, 0xC7, 0x61, 0x18, 0x0E, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x18, 0x60, 0xC7, 0x0C, 0x30, 0xE1, 0x87, 0x1C, 0x30, 0xC0, 0x80, 0x00, 0x00, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x10, 0x10, 0x08, 0x0C, 0x04, 0x06, 0x00, 0x00, 0x01, 0x07, 0x0C, 0x30, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xC0, 0x30, 0x1C, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x38, 0x78, 0x86, 0x03, 0x00, 0x04, 0x0E, 0x09, 0x18, 0x10, 0x20, 0x60, 0x40, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x38, 0x0C, 0x03, 0x01, 0x08, 0x1E, 0x31, 0x20, 0x40, 0x43, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x43, 0x40, 0x20, 0x21, 0x17, 0x1C, 0x00, 0x03, 0x07, 0x19, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x60, 0x20, 0x30, 0x10, 0x08, 0x08, 0x00, 0x00, 0x03, 0x06, 0x18, 0x70, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x60, 0x20, 0x30, 0x10, 0x18, 0x08, 0x08,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x02, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t bitmap_16x19[] PROGMEM = {  //Битмап буквы логотипа
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0x03, 0x06, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07,
};
const uint8_t bitmap_26x26_min[] PROGMEM = {  //Битмап знака минус
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x78, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t bitmap_26x26_plus[] PROGMEM = { //Битмап знака плюс
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t bitmap_128x64_colba[] PROGMEM = {//Битмап хроматографии
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xAB, 0xFF, 0xAB, 0x7F, 0xEB, 0x5F, 0xFB, 0x57, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1A, 0x7D, 0xF7, 0xFD, 0xEB, 0xFD, 0x17, 0x1D, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x7F, 0xFF, 0xFF, 0xFF, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC1, 0x01, 0x01, 0x01, 0x01, 0xC1, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE1, 0x01, 0x01, 0x01, 0x01, 0xE1, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE1, 0x01, 0x01, 0x01, 0x01, 0xE1, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0E, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFC, 0xF3, 0xB0, 0xF0, 0xB0, 0xD0, 0xF0, 0xB0, 0xF3, 0xBC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x38, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0E, 0x70, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xBE, 0x69, 0xE8, 0xB8, 0xB8, 0xE8, 0x58, 0xF8, 0x28, 0xF8, 0xC8, 0xF8, 0x59, 0xFE, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xBF, 0x57, 0xDE, 0x6B, 0xFE, 0x6B, 0xDF, 0xFA, 0x97, 0xFE, 0x6B, 0xBE, 0xEB, 0x5F, 0xF8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xC0, 0xB0, 0x8E, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x83, 0x8C, 0xB0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xBE, 0xFF, 0xAA, 0xF7, 0xDD, 0xF7, 0xDC, 0xF7, 0xEE, 0xDD, 0xFB, 0xD7, 0xFE, 0xD5, 0xBF, 0xF5, 0xAF, 0xFD, 0xAB, 0xFE, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0xDD, 0xF7, 0xDD, 0xF7, 0xDD, 0xF7, 0xED, 0xDA, 0xB6, 0xEF, 0xBD, 0xEB, 0xBE, 0xEB, 0xBD, 0xF5, 0xEF, 0xDE, 0xF8, 0xC0, 0x00, 0x00,
};
const uint8_t bitmap_2x19_water[] PROGMEM = { //Битмап капель
  0x38, 0x18,
  0xC7, 0xE7,
  0x00, 0x01,
};
const uint8_t ptr_bmp[] PROGMEM = {
  0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xE0, 0x40,
  0x01, 0x01, 0x01, 0x01, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00,
};
const uint8_t bitmap_11x11_Galka[] PROGMEM = {
  0xFF, 0x01, 0x21, 0xC1, 0x01, 0xC1, 0x31, 0x09, 0x05, 0x01, 0xFF,
  0x07, 0x04, 0x04, 0x04, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07,
};
const uint8_t bitmap_11x11_My_Gap[] PROGMEM = {
  0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF,
  0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07,
};
const uint8_t bitmap_11x11_Bare_Galka[] PROGMEM = {
  0x00, 0x00, 0x20, 0xC0, 0x00, 0xC0, 0x30, 0x08, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

bool sign;                         // Знак операции при изменегии процента заполнения колбы
bool lock;                         // Не даёт изменять процент наполнения колбы, пока не выберешь знак операции
bool anim_flag;                    // Не даёт несколько раз обновлять кадр до стирания
bool holl_flag;
bool pos_flag;
bool d_step = 0;
bool f_type;                         // Тип колбы
byte proc50ml;                      // Процент заполнения колбы 50 мл
byte proc25ml;                      // Процент заполнения колбы 25 мл
byte speaker_loud;                      // Громкость динамика
byte ay;                      // Хранит координаты для анимации
byte n_mode; // Хранит номер мода
byte flask_count;
byte delta_steps;
byte initializer_dev;
byte launcher_sta;
byte launcher_sto;
byte navigation_men;
byte parametr_reg;
byte contact_go;
byte settings_go;
byte initializer_dev2;
byte launcher_sta2;
byte launcher_sto2;
byte navigation_men2;
byte parametr_reg2;
byte contact_go2;
byte settings_go2;
int holl;
int holl_n;
int Min_U = 1023;
int New_U = 1023;
int Min_R = 1023;
int Min_L = 1023;
int New_R = 1023;
int New_L = 1023;

static int8_t pointer = 0; // Переменная указатель
static int8_t pointer2 = 0;
static int8_t pointer3 = 0;
static int8_t pointer4 = 0;
static int8_t pointer5 = 0;
static int8_t galka5 = 0;

uint32_t tmr_anim;         // переменная таймера анимации
uint32_t tmr_flask;         // переменная таймера времени простоя колбы под колонкой
uint32_t steper;         // переменная таймера шага
uint32_t laser_imp;         // переменная таймера импульса
uint32_t flask_act;         // переменная времени заполнения
uint32_t delayer;



void setup() {
  oled.init();
  oled.setPower(false);
  oled.setContrast(225); // Макс. яркость
  oled.setScale(2);
  pinMode(BIG_LASER, OUTPUT);
  pinMode(SMALL_LASERS, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(SWITCH_OFF, OUTPUT);
  pinMode(L_PHOTORESISTOR, INPUT);
  pinMode(R_PHOTORESISTOR, INPUT);
  pinMode(U_PHOTORESISTOR, INPUT);
  pinMode(HOLL_SENSOR, INPUT);
  digitalWrite(SWITCH_OFF, HIGH);
  digitalWrite(13, HIGH);
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  f_type = 0;
  EEPROM.get(1, proc25ml); //Чтение и присваивание переменным значений из памяти
  EEPROM.get(0, proc50ml);
  EEPROM.get(2, speaker_loud);
  EEPROM.get(3, n_mode);
  switch (n_mode) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
    case 0: Base(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
    case 2: Amongus(); break;
    case 4: PoleChydes(); break;
  }
  myDFPlayer.volume(speaker_loud * 5); //Set volume value. From 0 to 30
  myDFPlayer.play(random(initializer_dev, initializer_dev2)); //Play the first mp3
  oled.clear();           // Инциализация дисплея
  oled.update();
  oled.setPower(true); // экран включится
  Logo();
}

void loop() {
  /* кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
    pointer = constrain(pointer - 2, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
  }

  if (down.press() or down.hold()) {
    pointer = constrain(pointer + 2, 0, ITEMS - 1);
  }

  if (ok.press()) {   // Нажатие на ОК - переход в пункт
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: Launch(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 2: Settings();  myDFPlayer.play(random(navigation_men, navigation_men2)); break;
      case 4: BetterCallMe(); myDFPlayer.play(random(navigation_men, navigation_men2)); break;
    }
  }

  /* меню */
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол
  oled.print              // Вывод всех пунктов
  (F(
     " Запуск\n\r"   // Не забываем про '\n' - символ переноса строки
     " Настройки\n\r"
     " Контакты"
   ));
  printPointer(pointer);  // Вывод указателя
  oled.update();          // Выводим кадр на дисплей
}
void printPointer(uint8_t pointer) {
  // Указатель в начале строки
  oled.setCursor(0, pointer);

  oled.drawBitmap(0, pointer * 8, ptr_bmp, 11, 13);
}

void Launch(void) {
  flask_count = 0;
  digitalWrite(SWITCH_OFF, LOW);
  digitalWrite(BIG_LASER, HIGH);
  digitalWrite(SMALL_LASERS, HIGH);
  for (int p = 0;;) {
     if (micros() - steper >= SPEED) {
      p++;
      steper = micros();
      if (p % 2 == 0) {
        digitalWrite(PIN_STEP, HIGH);
      } else {
        digitalWrite(PIN_STEP, LOW);
      }
      if (p > 1000) {
        break;
      }
    }
  }
   for (;;) {
    if (micros() - steper >= SPEED) { //Средний шаг
      digitalWrite(PIN_STEP, HIGH);
    }
    if (micros() - steper >= SPEED * 2) {
      steper = micros();
      digitalWrite(PIN_STEP, LOW);
    }
    New_R = analogRead(R_PHOTORESISTOR);
    New_U = analogRead(U_PHOTORESISTOR);
    New_L = analogRead(L_PHOTORESISTOR);
    if (Min_L > New_L) {
      Min_L = New_L;
    }
    if (Min_R > New_R) {
      Min_R = New_R;
    }
    if (Min_U > New_U) {
      Min_U = New_U;
    }
    if (New_U - Min_U > 100) {
      Serial.print("Снятые показания: правый ");
      Serial.print(analogRead(R_PHOTORESISTOR));
      Serial.print(", левый ");
      Serial.print(analogRead(L_PHOTORESISTOR));
      Serial.print(", верхний ");
      Serial.print(analogRead(U_PHOTORESISTOR));
      Serial.println();
      Serial.print("Записанные показания: правый ");
      Serial.print(New_R);
      Serial.print(", левый ");
      Serial.print(New_L);
      Serial.print(", верхний ");
      Serial.print(New_U);
      Serial.println();
      Serial.print("Минимальные показания: правый ");
      Serial.print(Min_R);
      Serial.print(", левый ");
      Serial.print(Min_L);
      Serial.print(", верхний ");
      Serial.print(Min_U);
      Serial.println();
      pos_flag = true;
      if (Min_R < 350 and Min_L < 350 and Min_U < 300) {
        Serial.print("Пустой отсек");
        Serial.println();
        Serial.println();
      } else if (Min_R > 350) {
        Serial.print("Малая колба");
        Serial.println();
        Serial.println();
        delay(10000);
      } else {
        Serial.print("Большая колба");
        Serial.println();
        Serial.println();
        delay(10000);
      }
      Min_U = 1023;
      Min_R = 1023;
      Min_L = 1023;
      digitalWrite(SWITCH_OFF, HIGH);
      digitalWrite(SWITCH_OFF, LOW);
      for (int p = 0;;) {
        if (micros() - steper >= SPEED) {
          p++;
          steper = micros();
          if (p % 2 == 0) {
            digitalWrite(PIN_STEP, HIGH);
          } else {
            digitalWrite(PIN_STEP, LOW);
          }
          if (p > 1000) {
            break;
          }
        }
      }
    } 
  }
}
void Succes(void) {
  digitalWrite(SWITCH_OFF, HIGH);
  digitalWrite(BIG_LASER, LOW);
  digitalWrite(SMALL_LASERS, LOW);
  myDFPlayer.play(random(launcher_sto, launcher_sto2));
  oled.clear();
  oled.print
  (F(
     "Сортировка\n\r"   // Не забываем про '\n' - символ переноса строки
     "завершена"
   ));
  oled.update();
  for (;;) {
    ok.tick();
    if (ok.press()) {
      myDFPlayer.pause();
      return;
    }
  }
}
void Settings(void) {
  myDFPlayer.play(random(settings_go, settings_go2));
  for (;;) {
    /* Кнопки */
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();

    if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
      pointer2 = constrain(pointer2 - 2, 0, ITEMS2 - 1); // Двигаем указатель в пределах дисплея
    }

    if (down.press() or down.hold()) {
      pointer2 = constrain(pointer2 + 2, 0, ITEMS2 - 1);
    }

    if (ok.press()) {   // Нажатие на ОК - переход в пункт меню
      switch (pointer2) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
        case 0: myDFPlayer.play(random(navigation_men, navigation_men2)); Calibration(); myDFPlayer.play(random(navigation_men, navigation_men2)); break; // По нажатию на ОК при наведении на 0й пункт вызвать функцию
        case 2: myDFPlayer.play(random(navigation_men, navigation_men2)); Sound(); myDFPlayer.play(random(navigation_men, navigation_men2)); break;
      }
    }
    /* меню */
    oled.clear();           // Очищаем буфер
    oled.home();            // Курсор в левый верхний угол
    oled.print              // Вывод всех пунктов
    (F(
       " Процент\n\r"   // Не забываем про '\n' - символ переноса строки
       " Звук"
     ));
    printPointer2(pointer2);  // Вывод указателя
    oled.update();          // Выводим кадр на дисплей
    back.tick();
    if (back.press()) return; // return возвращает нас в предыдущее меню
  }
}
void printPointer2(uint8_t pointer2) {
  // Указатель в начале строки
  oled.setCursor(0, pointer2);
  oled.drawBitmap(0, pointer2 * 8, ptr_bmp, 11, 13);
}

void BetterCallMe(void) {
  myDFPlayer.play(random(contact_go, contact_go2));
  oled.setScale(1);
  oled.clear();
  oled.home();
  oled.print(F(
               "Если при эксплуатации\n\r"
               "устройства возникли\n\r"
               "проблемы или у вас\n\r"
               "есть предложения по\n\r"
               "его улучшению пишите\n\r"
               "на эту почту\n\r"
               "\n\r"
               "Rozmusel@yandex.ru"
             ));
  oled.update();
  while (1) {
    back.tick();
    if (back.press()) {
      oled.setScale(2);
      return; // return возвращает нас в предыдущее меню
    }
  }
}
void Calibration(void) {
  for (;;) {
    /* Кнопки */
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();

    if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
      pointer3 = constrain(pointer3 - 3, 0, ITEMS3 - 1); // Двигаем указатель в пределах дисплея
    }

    if (down.press() or down.hold()) {
      pointer3 = constrain(pointer3 + 2, 0, ITEMS3 - 1);
    }

    if (ok.press()) {   // Нажатие на ОК - переход в пункт меню
      switch (pointer3) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
        case 0: myDFPlayer.play(random(navigation_men, navigation_men2)); F50ml(); myDFPlayer.play(random(navigation_men, navigation_men2)); break; // По нажатию на ОК при наведении на 0й пункт вызвать функцию
        case 2: myDFPlayer.play(random(navigation_men, navigation_men2)); F25ml(); myDFPlayer.play(random(navigation_men, navigation_men2)); break;
      }
    }
    /* меню */
    oled.clear();           // Очищаем буфер
    oled.home();            // Курсор в левый верхний угол
    oled.print              // Вывод всех пунктов
    (F(
       " 50мл\n\r"   // Не забываем про '\n' - символ переноса строки
       " 25мл"
     ));
    printPointer3(pointer3);  // Вывод указателя
    oled.update();          // Выводим кадр на дисплей
    back.tick();
    if (back.press()) return; // return возвращает нас в предыдущее меню
  }
}
void printPointer3(uint8_t pointer3) {
  // Указатель в начале строки
  oled.setCursor(0, pointer3);
  oled.drawBitmap(0, pointer3 * 8, ptr_bmp, 11, 13);
}
void F25ml(void) {
  oled.setScale(1);
  oled.clear();
  oled.home();
  oled.update(); oled.print(F(
                              "        Колба 25 мл\n\r"
                              "\n\r"
                              "\n\r"
                              "        Заполнение\n\r"
                              "\n\r"
                              "            "
                            ));
  oled.print(proc25ml);
  oled.print("%");
  oled.drawBitmap(4, 34, bitmap_26x26_min, 26, 26);
  oled.drawBitmap(4, 4, bitmap_26x26_plus, 26, 26);
  oled.update();
  lock = 0;
  for (;;) {
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();
    back.tick();
    if (down.press()) {
      oled.roundRect(2, 2, 32, 32, OLED_CLEAR);
      oled.drawBitmap(4, 4, bitmap_26x26_plus, 26, 26);
      oled.roundRect(2, 32, 32, 62, OLED_STROKE);
      oled.update();
      sign = 1;
      lock = 1;
    }
    if (up.press()) {
      oled.roundRect(2, 32, 32, 62, OLED_CLEAR);
      oled.drawBitmap(4, 34, bitmap_26x26_min, 26, 26);
      oled.roundRect(2, 2, 32, 32, OLED_STROKE);
      oled.update();
      sign = 0;
      lock = 1;
    }
    if (ok.press() && proc25ml >= 60 && proc25ml <= 100 && lock == 1) {
      if (sign == 0 && proc25ml < 100) {
        proc25ml = proc25ml + 5;
      } if (sign == 1 && proc25ml > 60) {
        proc25ml = proc25ml - 5;
      }
      oled.setCursorXY(71, 40); // курсор в (пиксель X, пиксель Y)
      oled.print("    ");
      oled.setCursorXY(71, 40); // курсор в (пиксель X, пиксель Y)
      oled.print(proc25ml);
      oled.print("%");
      oled.update();
    }
    if (back.press()) {
      EEPROM.update(1, proc25ml);
      oled.setScale(2);
      return; // return возвращает нас в предыдущее меню
    }
  }
}
void F50ml(void) {
  oled.setScale(1);
  oled.clear();
  oled.home();
  oled.update(); oled.print(F(
                              "        Колба 50 мл\n\r"
                              "\n\r"
                              "\n\r"
                              "        Заполнение\n\r"
                              "\n\r"
                              "            "
                            ));
  oled.print(proc50ml);
  oled.print("%");
  oled.drawBitmap(4, 34, bitmap_26x26_min, 26, 26);
  oled.drawBitmap(4, 4, bitmap_26x26_plus, 26, 26);
  oled.update();
  lock = 0;
  for (;;) {
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();
    back.tick();
    if (down.press()) {
      oled.roundRect(2, 2, 32, 32, OLED_CLEAR);
      oled.drawBitmap(4, 4, bitmap_26x26_plus, 26, 26);
      oled.roundRect(2, 32, 32, 62, OLED_STROKE);
      oled.update();
      sign = 1;
      lock = 1;
    }
    if (up.press()) {
      oled.roundRect(2, 32, 32, 62, OLED_CLEAR);
      oled.drawBitmap(4, 34, bitmap_26x26_min, 26, 26);
      oled.roundRect(2, 2, 32, 32, OLED_STROKE);
      oled.update();
      sign = 0;
      lock = 1;
    }
    if (ok.press() && proc50ml >= 40 && proc50ml <= 100 && lock == 1) {
      if (sign == 0 && proc50ml < 100) {
        proc50ml = proc50ml + 5;
      } if (sign == 1 && proc50ml > 40) {
        proc50ml = proc50ml - 5;
      }
      oled.setCursorXY(71, 40); // курсор в (пиксель X, пиксель Y)
      oled.print("    ");
      oled.setCursorXY(71, 40); // курсор в (пиксель X, пиксель Y)
      oled.print(proc50ml);
      oled.print("%");
      oled.update();
    }
    if (back.press()) {
      EEPROM.update(0, proc50ml);
      oled.setScale(2);
      return; // return возвращает нас в предыдущее меню
    }
  }
}
void Sound(void) {
  for (;;) {
    /* Кнопки */
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();
    back.tick();

    if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
      pointer4 = constrain(pointer4 - 2, 0, ITEMS4 - 1); // Двигаем указатель в пределах дисплея
    }

    if (down.press() or down.hold()) {
      pointer4 = constrain(pointer4 + 2, 0, ITEMS4 - 1);
    }

    if (ok.press()) {   // Нажатие на ОК - переход в пункт меню
      switch (pointer4) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
        case 0: myDFPlayer.play(random(navigation_men, navigation_men2)); Loud(); myDFPlayer.play(random(navigation_men, navigation_men2)); break; // По нажатию на ОК при наведении на 0й пункт вызвать функцию
        case 2: myDFPlayer.play(random(navigation_men, navigation_men2)); Mods(); myDFPlayer.play(random(navigation_men, navigation_men2)); break;
      }
    }
    if (back.press()) return; // return возвращает нас в предыдущее меню

    /* меню */
    oled.clear();           // Очищаем буфер
    oled.home();            // Курсор в левый верхний угол
    oled.print              // Вывод всех пунктов
    (F(
       " Громкость\n\r"   // Не забываем про '\n' - символ переноса строки
       " Моды"
     ));
    printPointer4(pointer4);  // Вывод указателя
    oled.update();          // Выводим кадр на дисплей
  }
}
void printPointer4(uint8_t pointer4) {
  // Указатель в начале строки
  oled.setCursor(0, pointer4);

  oled.drawBitmap(0, pointer4 * 8, ptr_bmp, 11, 13);
}
void Loud(void) {
  oled.setScale(1);
  oled.clear();
  oled.home();
  oled.update(); oled.print(F(
                              "         Динамики\n\r"
                            ));
  oled.drawBitmap(4, 34, bitmap_26x26_min, 26, 26);
  oled.drawBitmap(4, 4, bitmap_26x26_plus, 26, 26);
  switch (speaker_loud) {
    case 0:
      oled.rect(45, 50, 55, 60, OLED_STROKE);
      oled.rect(60, 40, 70, 60, OLED_STROKE);
      oled.rect(75, 30, 85, 60, OLED_STROKE);
      oled.rect(90, 20, 100, 60, OLED_STROKE);
      oled.rect(105, 10, 115, 60, OLED_STROKE);
      break;
    case 1:
      oled.rect(45, 50, 55, 60, OLED_FILL);
      oled.rect(60, 40, 70, 60, OLED_STROKE);
      oled.rect(75, 30, 85, 60, OLED_STROKE);
      oled.rect(90, 20, 100, 60, OLED_STROKE);
      oled.rect(105, 10, 115, 60, OLED_STROKE);
      break;
    case 2:
      oled.rect(45, 50, 55, 60, OLED_FILL);
      oled.rect(60, 40, 70, 60, OLED_FILL);
      oled.rect(75, 30, 85, 60, OLED_STROKE);
      oled.rect(90, 20, 100, 60, OLED_STROKE);
      oled.rect(105, 10, 115, 60, OLED_STROKE);
      break;
    case 3:
      oled.rect(45, 50, 55, 60, OLED_FILL);
      oled.rect(60, 40, 70, 60, OLED_FILL);
      oled.rect(75, 30, 85, 60, OLED_FILL);
      oled.rect(90, 20, 100, 60, OLED_STROKE);
      oled.rect(105, 10, 115, 60, OLED_STROKE);
      break;
    case 4:
      oled.rect(45, 50, 55, 60, OLED_FILL);
      oled.rect(60, 40, 70, 60, OLED_FILL);
      oled.rect(75, 30, 85, 60, OLED_FILL);
      oled.rect(90, 20, 100, 60, OLED_FILL);
      oled.rect(105, 10, 115, 60, OLED_STROKE);
      break;
    case 5:
      oled.rect(45, 50, 55, 60, OLED_FILL);
      oled.rect(60, 40, 70, 60, OLED_FILL);
      oled.rect(75, 30, 85, 60, OLED_FILL);
      oled.rect(90, 20, 100, 60, OLED_FILL);
      oled.rect(105, 10, 115, 60, OLED_FILL);
      break;
  }
  oled.update();
  lock = 0;
  for (;;) {
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();
    back.tick();
    if (down.press()) {
      oled.roundRect(2, 2, 32, 32, OLED_CLEAR);
      oled.drawBitmap(4, 4, bitmap_26x26_plus, 26, 26);
      oled.roundRect(2, 32, 32, 62, OLED_STROKE);
      oled.update();
      sign = 1;
      lock = 1;
    }
    if (up.press()) {
      oled.roundRect(2, 32, 32, 62, OLED_CLEAR);
      oled.drawBitmap(4, 34, bitmap_26x26_min, 26, 26);
      oled.roundRect(2, 2, 32, 32, OLED_STROKE);
      oled.update();
      sign = 0;
      lock = 1;
    }
    if (ok.press() && speaker_loud >= 0 && speaker_loud <= 5 && lock == 1) {
      if (sign == 0 && speaker_loud < 5) {
        speaker_loud = speaker_loud + 1;
      } if (sign == 1 && speaker_loud > 0) {
        speaker_loud = speaker_loud - 1;
      }
      oled.setCursorXY(71, 40); // курсор в (пиксель X, пиксель Y)
      myDFPlayer.volume(speaker_loud * 5);
      myDFPlayer.play(random(parametr_reg, parametr_reg2));
      switch (speaker_loud) {
        case 0:
          oled.rect(45, 50, 55, 60, OLED_CLEAR);
          oled.rect(45, 50, 55, 60, OLED_STROKE);
          break;
        case 1:
          oled.rect(45, 50, 55, 60, OLED_FILL);
          oled.rect(60, 40, 70, 60, OLED_CLEAR);
          oled.rect(60, 40, 70, 60, OLED_STROKE);
          break;
        case 2:
          oled.rect(60, 40, 70, 60, OLED_FILL);
          oled.rect(75, 30, 85, 60, OLED_CLEAR);
          oled.rect(75, 30, 85, 60, OLED_STROKE);
          break;
        case 3:
          oled.rect(75, 30, 85, 60, OLED_FILL);
          oled.rect(90, 20, 100, 60, OLED_CLEAR);
          oled.rect(90, 20, 100, 60, OLED_STROKE);
          break;
        case 4:
          oled.rect(90, 20, 100, 60, OLED_FILL);
          oled.rect(105, 10, 115, 60, OLED_CLEAR);
          oled.rect(105, 10, 115, 60, OLED_STROKE);
          break;
        case 5:
          oled.rect(105, 10, 115, 60, OLED_FILL);
          break;
      }
      oled.update();
    }
    if (back.press()) {
      EEPROM.update(2, speaker_loud);
      oled.setScale(2);
      return; // return возвращает нас в предыдущее меню
    }
  }
}
void Mods(void) {
  for (;;) {
    /* Кнопки */
    up.tick();                 // Опрос кнопок
    down.tick();
    ok.tick();
    back.tick();

    if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
      pointer5 = constrain(pointer5 - 2, 0, ITEMS5 - 1); // Двигаем указатель в пределах дисплея
    }

    if (down.press() or down.hold()) {
      pointer5 = constrain(pointer5 + 2, 0, ITEMS5 - 1);
    }

    if (ok.press()) {   // Нажатие на ОК - переход в пункт меню
      switch (pointer5) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
        case 0: n_mode = pointer5; Base(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
        case 2: n_mode = pointer5; Amongus(); break;
        case 4: n_mode = pointer5; PoleChydes(); break;
      }
    }
    if (back.press()) {
      EEPROM.update(3, n_mode);  // return возвращает нас в предыдущее меню
      return;
    }

    /* меню */
    oled.clear();           // Очищаем буфер
    oled.home();            // Курсор в левый верхний угол
    oled.print              // Вывод всех пунктов
    (F(
       " Базовый\n\r"   // Не забываем про '\n' - символ переноса строки
       " Among Us\n\r"
       " Поле чудес"
     ));
    printPointer5(pointer5);  // Вывод указателя
    oled.update();          // Выводим кадр на дисплей
  }
}
void printPointer5(uint8_t pointer5) {
  // Указатель в начале строки
  if (n_mode == pointer5) {
    oled.drawBitmap(0, pointer5 * 8 + 2, bitmap_11x11_Galka, 11, 11);
  } else {
    oled.drawBitmap(0, pointer5 * 8 + 2, bitmap_11x11_My_Gap, 11, 11);
    oled.drawBitmap(0, n_mode * 8 + 2, bitmap_11x11_Bare_Galka, 11, 11);
  }
}

void Logo(void) {
  oled.drawBitmap(1, 19, bitmap_127x27, 127, 27);
  oled.update();
  delay(1000);
  ay = 64;
  for (;;) {
    ay = ay - 2;
    oled.drawBitmap(56, ay, bitmap_16x19, 16, 19);
    oled.line(56, ay + 19, 72, ay + 19, OLED_CLEAR);
    oled.line(56, ay + 20, 72, ay + 20, OLED_CLEAR);
    oled.dot(56, ay + 18, OLED_CLEAR);
    oled.update();
    if (ay < 28) {
      delay(1000);
      return;
    }
  }
}
void WatchFlask(void) {
  digitalWrite(SMALL_LASERS, LOW);
  digitalWrite(BIG_LASER, LOW);
  delayer = millis();
  tmr_flask = millis();
  for (;;) {
    if (millis() - delayer >= 1000) {
      digitalWrite(BIG_LASER, HIGH);
      delay(100);
      delayer = millis();
      if (analogRead(U_PHOTORESISTOR) > 625) {
        Waiter();
        return;
      }
      digitalWrite(BIG_LASER, LOW);
    }
    if (millis() - tmr_anim >= 500 && anim_flag == 0) {    // сброс таймера
      anim_flag = 1;
      oled.line(63, 27, 63, 47, OLED_CLEAR);
      oled.line(64, 27, 64, 47, OLED_CLEAR);
      oled.drawBitmap(63, 27, bitmap_2x19_water, 2, 19, BITMAP_NORMAL);
      oled.update(63, 27, 65, 47);
    }
    if (millis() - tmr_anim >= 1000) {
      tmr_anim = millis();                   // сброс таймера
      anim_flag = 0;
      oled.line(64, 27, 64, 47, OLED_CLEAR);
      oled.line(63, 27, 63, 47, OLED_CLEAR);
      oled.drawBitmap(63, 27, bitmap_2x19_water, 2, 19, BITMAP_INVERT);
      oled.update(63, 27, 65, 47);
    }
  }
}
void Waiter(void) {
  digitalWrite(BIG_LASER, LOW);
  if (f_type == 0) {
    flask_act = ((proc50ml * (millis() - tmr_flask)) / 20);
  } else {
    flask_act = ((proc25ml * (millis() - tmr_flask)) / 60);
  }
  for (;;) {
    if (millis() - tmr_flask >= flask_act) {
      digitalWrite(SMALL_LASERS, HIGH);
      digitalWrite(BIG_LASER, HIGH);
      return;
    }
    if (millis() - tmr_anim >= 500 && anim_flag == 0) {    // сброс таймера
      anim_flag = 1;
      oled.line(63, 27, 63, 47, OLED_CLEAR);
      oled.line(64, 27, 64, 47, OLED_CLEAR);
      oled.drawBitmap(63, 27, bitmap_2x19_water, 2, 19, BITMAP_NORMAL);
      oled.update(63, 27, 65, 47);
    }
    if (millis() - tmr_anim >= 1000) {
      tmr_anim = millis();                   // сброс таймера
      anim_flag = 0;
      oled.line(64, 27, 64, 47, OLED_CLEAR);
      oled.line(63, 27, 63, 47, OLED_CLEAR);
      oled.drawBitmap(63, 27, bitmap_2x19_water, 2, 19, BITMAP_INVERT);
      oled.update(63, 27, 65, 47);
    }
  }
}
void df_step(void) {
  for (int k = 0; k < 100000000; k++) {
    if (millis() - steper >= SPEED) {
      digitalWrite(PIN_STEP, HIGH);
    }
    if (millis() - steper >= SPEED * 2) {
      steper = millis();
      digitalWrite(PIN_STEP, LOW);
    }
  }

}
void Base(void) {
  initializer_dev = 1;
  launcher_sta = 2;
  launcher_sto = 3;
  navigation_men = 4;
  parametr_reg = 4;
  contact_go = 4;
  settings_go = 4;

  initializer_dev2 = 2;
  launcher_sta2 = 3;
  launcher_sto2 = 4;
  navigation_men2 = 5;
  parametr_reg2 = 5;
  contact_go2 = 5;
  settings_go2 = 5;
}
void Amongus(void) {
  initializer_dev = 6;
  launcher_sta = 7;
  launcher_sto = 9;
  navigation_men = 12;
  parametr_reg = 11;
  contact_go = 13;
  settings_go = 12;

  initializer_dev2 = 7;
  launcher_sta2 = 9;
  launcher_sto2 = 11;
  navigation_men2 = 13;
  parametr_reg2 = 12;
  contact_go2 = 14;
  settings_go2 = 13;
}
void PoleChydes(void) {
  initializer_dev = 14;
  launcher_sta = 15;
  launcher_sto = 19;
  navigation_men = 5;
  parametr_reg = 5;
  contact_go = 24;
  settings_go = 23;

  initializer_dev2 = 15;
  launcher_sta2 = 19;
  launcher_sto2 = 23;
  navigation_men2 = 6;
  parametr_reg2 = 6;
  contact_go2 = 26;
  settings_go2 = 24;

}
