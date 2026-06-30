#define H_SPEED 2400
#define M_SPEED 3000
#define S_SPEED 15000
#define L_PHOTORESISTOR_TRESHOLD 700
#define R_PHOTORESISTOR_TRESHOLD 400
#define U_PHOTORESISTOR_TRESHOLD 600

#define BIG_LASER 3
#define SMALL_LASERS 4

#define R_PHOTORESISTOR A2
#define L_PHOTORESISTOR A1
#define U_PHOTORESISTOR A0
#define HOLL_SENSOR A3

#define PIN_DIR 5
#define PIN_STEP 6
#define SWITCH_OFF 8

int Min_U = 1023;
int New_U = 1023;
int Min_R = 1023;
int Min_L = 1023;
int New_R = 1023;
int New_L = 1023;

bool pos_flag = false;

uint32_t steper;

void setup() {
  pinMode(BIG_LASER, OUTPUT);
  pinMode(SMALL_LASERS, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);

  pinMode(L_PHOTORESISTOR, INPUT);
  pinMode(R_PHOTORESISTOR, INPUT);
  pinMode(U_PHOTORESISTOR, INPUT);
  pinMode(HOLL_SENSOR, INPUT);
  pinMode(SWITCH_OFF, OUTPUT);
  digitalWrite(SWITCH_OFF, HIGH);

  digitalWrite(BIG_LASER, HIGH);
  digitalWrite(SMALL_LASERS, HIGH);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
      digitalWrite(SWITCH_OFF, HIGH);

  Serial.begin(9600);
  steper = micros();
}

void loop() {
  for (;;) {
    if (micros() - steper >= M_SPEED) { //Средний шаг
      digitalWrite(PIN_STEP, HIGH);
    }
    if (micros() - steper >= M_SPEED * 2) {
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
    if (New_U - Min_U > 500) {
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
        if (micros() - steper >= M_SPEED) {
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
  Serial.print(',');

}
