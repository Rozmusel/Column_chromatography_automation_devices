#define H_SPEED 2400
#define M_SPEED 8000
#define S_SPEED 15000
#define Delta 50
#define HOLL_TRESHOLD_DOWN 310
#define HOLL_TRESHOLD_UP 425

#define BIG_LASER 3
#define SMALL_LASERS 4

#define R_PHOTORESISTOR A2
#define L_PHOTORESISTOR A1
#define U_PHOTORESISTOR A0
#define HOLL_SENSOR A3

#define PIN_DIR 5
#define PIN_STEP 6

int Max_H = 0;
int New_H = 0;

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
  New_H = analogRead(HOLL_SENSOR);
  if (New_H > HOLL_TRESHOLD_UP and pos_flag == true) {
    pos_flag = false;
  }
  if (Max_H > New_H){
    Max_H = New_H;
  }
  if (Max_H < New_H and New_H < HOLL_TRESHOLD_DOWN and pos_flag == false){
    delay(1000);
    pos_flag = true;
  }
}
