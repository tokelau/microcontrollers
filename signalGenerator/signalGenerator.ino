#define PIN_9   9 // генерация
#define PIN_10  10 // отслеживание

int curTime = 0;
int oldTimeGenerate = 0;

// принимаем
bool oldSignal = 0;
bool curSignal = 0;
// отправляемые сигналы
bool sendSignal = 1;

// отслеживаем
float T = 1000;
float t = 500;
float d = t/ T;
int oldTimeDetecting_T = 0;
int oldTimeDetecting_t = 0;

void setup() {

  pinMode(PIN_9, OUTPUT);
  pinMode(PIN_10, INPUT);
  
  Serial.begin(9600);
  oldTimeGenerate = millis();
  oldTimeDetecting_T = millis();
  oldTimeDetecting_t = millis();

}

void loop() {
  //генерация
  curTime = millis();
  if ((curTime - oldTimeGenerate) >= 1000)
  {
     oldTimeGenerate = millis();
     sendSignal = 1;
  }
  if ((curTime - oldTimeGenerate) >= 500)
  {
    sendSignal = 0;
  }
  digitalWrite(PIN_9, sendSignal);
  
  //чтение
   curSignal = digitalRead(PIN_10);
   if (oldSignal != curSignal){
      if (curSignal == 0)
      {
        Serial.println("1 to 0");
        t = curTime - oldTimeDetecting_T;
      } else if (curSignal == 1) {
        Serial.println("0 to 1");
        T = curTime - oldTimeDetecting_T;
        oldTimeDetecting_T = millis();
        oldTimeDetecting_t = millis();
      }
    
   }

   d = t / T;
   Serial.print("T = ");
   Serial.print(T);
   Serial.print( "  | t = ");
   Serial.print(t);
   Serial.print(", then d = ");
   Serial.println(d);
   oldSignal = curSignal;
  
}
