#define H_SPEED 2400
#define M_SPEED 1000
#define S_SPEED 15000
#define Delta 50
#define L_PHOTORESISTOR_TRESHOLD 450
#define R_PHOTORESISTOR_TRESHOLD 410
#define U_PHOTORESISTOR_TRESHOLD 800
#define U_TRESHOLD_DOWN 900
#define U_TRESHOLD_UP 1000

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

  digitalWrite(BIG_LASER, HIGH);
  digitalWrite(SMALL_LASERS, HIGH);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(9600);
}

void loop() {
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
  if (New_U > U_TRESHOLD_UP and pos_flag == true) {
    pos_flag = false;
  }
  if (Min_U > New_U) {
    Min_U = New_U;
  }
  if (New_U < U_TRESHOLD_DOWN and pos_flag == false and New_U - Min_U > 550) {
    delay(1000);
    pos_flag = true;
    Serial.print(analogRead(analogRead(HOLL_SENSOR)));
    Serial.print(',');
    Serial.print(Min_R);
    Serial.print(',');
    Serial.print(Min_L);
    Serial.print(',');
    Serial.print(Min_U);
    if (Min_R > R_PHOTORESISTOR_TRESHOLD) {
      Serial.print(" Маленькая");
    } else if (Min_L > L_PHOTORESISTOR_TRESHOLD) {
      Serial.print(" Большая");
    } else {
      Serial.print(" Пусто");
    }
    Serial.println();
    Min_U = 1023;
    Min_R = 1023;
    Min_L = 1023;
  digitalWrite(SWITCH_OFF, HIGH);
  delay(10000);
  digitalWrite(SWITCH_OFF, LOW);
  }
}
