// Установка выводов
const int dirPin = 5;
const int stepPin = 6;
int spede = 1000;
uint32_t steper;         // переменная таймера шага
uint32_t delo;         // переменная таймера шага

void setup()
{
  // Объявить контакты как выходы
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  }
void loop()
{
  spede = spede + 500;
  delo = millis();
  Serial.print(spede);
  while (millis() - delo <= 10000) {
    if (micros() - steper >= spede) {
      digitalWrite(stepPin, HIGH);
    }
    if (micros() - steper >= spede * 2) {
      steper = micros();
      digitalWrite(stepPin, LOW);
    }
  }
}
