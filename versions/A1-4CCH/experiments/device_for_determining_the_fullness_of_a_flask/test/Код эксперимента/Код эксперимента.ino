#define las A0 //считывание с делителя на фоторезисторе
#define reg A1 //считывание с делителя напотенциометре 
#define but 3 //считывание кнопки
#define power 2 //питание лазера
bool flag = false;
uint32_t btnTimer = 0;
int k; // напряжение от делителе на потенциометре
int light; // напряжение от делителя на лазере
void setup() {
Serial.begin(9600);
pinMode(reg, INPUT);
pinMode(A0, INPUT);
pinMode(power, OUTPUT);
pinMode(but, INPUT_PULLUP);
}

void loop() {
  bool btnState = !digitalRead(3);
  if (btnState && !flag && millis() - btnTimer > 100) {
    flag = true;
    btnTimer = millis();
  }
  if (btnState && flag && millis() - btnTimer > 500) {
    btnTimer = millis();
  }
  if (!btnState && flag && millis() - btnTimer > 500) {
    flag = false;
    btnTimer = millis();
  }
if(flag == 1){
int Timer1 = micros();
digitalWrite(2, HIGH);
k = analogRead(reg);
light = analogRead(las);
if(k > light){
  Serial.println("Колба наполнена");
}else{
  Serial.println("Колба не заполнена");
}
digitalWrite(2, LOW);
int Timer2 = micros();
int timer3 = Timer2-Timer1;
Serial.print("Время работы лазера: ");
Serial.print(timer3);
Serial.println(" микросекунд");
for (;;) {
bool btnState = !digitalRead(3);
  if (btnState && !flag && millis() - btnTimer > 100) {
    flag = true;
    btnTimer = millis();
  }
  if (btnState && flag && millis() - btnTimer > 500) {
    btnTimer = millis();
  }
  if (!btnState && flag && millis() - btnTimer > 500) {
    flag = false;
    btnTimer = millis();
  }
  if (flag == 0) break;
}
}
}
