#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
#include <GyverOLED.h>          // Библиотека дисплея
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
void Launch();
void Settings();
void BetterCallMe();

#include <EncButton2.h>
EncButton2<EB_BTN> btn[4];
EncButton2<EB_BTN> back(INPUT_PULLUP, 2);
EncButton2<EB_BTN> ok(INPUT_PULLUP, 3);
EncButton2<EB_BTN> down(INPUT_PULLUP, 4);
EncButton2<EB_BTN> up(INPUT_PULLUP, 5);



#define ITEMS 5

const uint8_t ptr_bmp[] PROGMEM = {
  0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0x40, 
  0x01, 0x01, 0x01, 0x01, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 
};

void setup() {
  oled.setScale(2);
  oled.init();           // Инциализация дисплея
  oled.setContrast(255); // Макс. яркость
}

void loop() {
  static int8_t pointer = 0; // Переменная указатель
  /* Кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.state() or up.hold()) {                // Если кнопку нажали или удерживают
    pointer = constrain(pointer - 2, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
  }

  if (down.state() or down.hold()) {
    pointer = constrain(pointer + 2, 0, ITEMS - 1);
  }

  if (ok.state()) {   // Нажатие на ОК - переход в пункт меню
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: Launch(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 2: Settings(); break;
      case 4: BetterCallMe(); break;
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

/* пример вложеной функции, которую можно вызвать из под меню */
void Launch(void) {
  oled.clear();
  oled.home();
  oled.print(F("11"));
  oled.update();
  while (1) {
    back.tick();
    if (back.state()) return; // return возвращает нас в предыдущее меню
  }
}
void Settings(void) {
  oled.clear();
  oled.home();
  oled.print(F("22"));
  oled.update();
  while (1) {
    back.tick();
    if (back.state()) return; // return возвращает нас в предыдущее меню
  }
}

void BetterCallMe(void) {
  oled.clear();
  oled.home();
  oled.print(F("33"));
  oled.update();
  while (1) {
    back.tick();
    if (back.state()) return; // return возвращает нас в предыдущее меню
  }
}
