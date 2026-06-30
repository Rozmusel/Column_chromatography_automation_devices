#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
#include <GyverOLED.h>          // Библиотека дисплея
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
void Launch();
void Settings();
void BetterCallMe();
void Calibration();
void Sound();

#include <EncButton2.h>
EncButton2<EB_BTN> btn[4];
EncButton2<EB_BTN> back(INPUT_PULLUP, 2);
EncButton2<EB_BTN> ok(INPUT_PULLUP, 3);
EncButton2<EB_BTN> down(INPUT_PULLUP, 4);
EncButton2<EB_BTN> up(INPUT_PULLUP, 5);



#define ITEMS 5
#define ITEMS2 3

#define EB_DEB 50       // дебаунс кнопки, мс
#define EB_CLICK 50// таймаут накликивания кнопки, мс

const uint8_t ptr_bmp[] PROGMEM = {
  0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0x40, 
  0x01, 0x01, 0x01, 0x01, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 
};

void setup() {
  oled.setScale(2);
  oled.init();           // Инциализация дисплея
  oled.setContrast(255); // Макс. яркость
  Serial.begin(115200);
}

void loop() {
  bool change = 0;
  static int8_t pointer = 0; // Переменная указатель
  /* Кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
    pointer = constrain(pointer - 2, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
    change = 1;
  }

  if (down.press() or down.hold()) {
    pointer = constrain(pointer + 2, 0, ITEMS - 1);
    change = 1;
  }

  if (ok.press()) {   // Нажатие на ОК - переход в пункт меню
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
    if (back.press()) return; // return возвращает нас в предыдущее меню
  }
}
void Settings(void) { 
  for (;;) {
  bool change2 = 0;
  static int8_t pointer2 = 0; // Переменная указатель
  /* Кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.press() or up.hold()) {                // Если кнопку нажали или удерживают
    pointer2 = constrain(pointer2 - 2, 0, ITEMS2 - 1); // Двигаем указатель в пределах дисплея
    change2 = 1;
  }

  if (down.press() or down.hold()) {
    pointer2 = constrain(pointer2 + 2, 0, ITEMS2 - 1);
    change2 = 1;
  }

  if (ok.press()) {   // Нажатие на ОК - переход в пункт меню
    switch (pointer2) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: Calibration(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 2: Sound(); break;
    }
  }
  /* меню */
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол
  oled.print              // Вывод всех пунктов
  (F(
     " Заполнение\n\r"   // Не забываем про '\n' - символ переноса строки
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
  oled.clear();
  oled.home();
  oled.print(F("33"));
  oled.update();
  while (1) {
    back.tick();
    if (back.press()) return; // return возвращает нас в предыдущее меню
  }
}
void Calibration(void) {
  oled.clear();
  oled.home();
  oled.print(F("44"));
  oled.update();
  while (1) {
    back.tick();
    if (back.press()) return; // return возвращает нас в предыдущее меню
  }
}
void Sound(void) {
  oled.clear();
  oled.home();
  oled.print(F("55"));
  oled.update();
  while (1) {
    back.tick();
    if (back.press()) return; // return возвращает нас в предыдущее меню
  }
}
