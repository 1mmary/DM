const int SEG[7] = {5, 13, 10, 8, 7, 4, 11};
const int DP_PIN = 9;
const int DIG[4] = {6, 3, 2, 12};
const int BTN    = A0;

//              A  B  C  D  E  F  G
const byte N[10][7] = {
  {0,0,0,0,0,0,1}, // 0
  {1,0,0,1,1,1,1}, // 1
  {0,0,1,0,0,1,0}, // 2
  {0,0,0,0,1,1,0}, // 3
  {1,0,0,1,1,0,0}, // 4
  {0,1,0,0,1,0,0}, // 5
  {0,1,0,0,0,0,0}, // 6
  {0,0,0,1,1,1,1}, // 7
  {0,0,0,0,0,0,0}, // 8
  {0,0,0,0,1,0,0}, // 9
};

// Режимы:
// 0 = показ "5555" (старт экран)
// 1 = счёт идёт
// 2 = счёт остановлен
// 3 = сброс (показ "5555")

int  mode    = 0;
int  secs    = 0;
bool dpBlink = false;

unsigned long tSec = 0;
unsigned long tDp  = 0;
bool prevBtn = HIGH;
unsigned long tBtn = 0;

void showPos(int pos, int digit, bool dot) {
  for (int i = 0; i < 4; i++) digitalWrite(DIG[i], LOW);
  for (int s = 0; s < 7; s++) digitalWrite(SEG[s], N[digit][s]);
  digitalWrite(DP_PIN, dot ? LOW : HIGH);
  digitalWrite(DIG[pos], HIGH);
  delayMicroseconds(2500);
  digitalWrite(DIG[pos], LOW);
}

void showNumber(int num, bool dot) {
  showPos(0,  num / 1000,       dot);
  showPos(1, (num / 100) % 10,  dot);
  showPos(2, (num / 10)  % 10,  dot);
  showPos(3,  num % 10,         dot);
}

// Показать "5555"
void showFives() {
  showPos(0, 5, false);
  showPos(1, 5, false);
  showPos(2, 5, false);
  showPos(3, 5, false);
}

void onPress() {
  if (mode == 0) {          // старт экран → запуск счёта
    secs    = 0;
    dpBlink = false;
    tSec    = millis();
    tDp     = millis();
    mode    = 1;
  } else if (mode == 1) {   // счёт → стоп
    mode = 2;
  } else if (mode == 2) {   // стоп → сброс "5555"
    mode = 3;
  } else if (mode == 3) {   // сброс → снова счёт
    secs    = 0;
    dpBlink = false;
    tSec    = millis();
    tDp     = millis();
    mode    = 1;
  }
}

void setup() {
  for (int s = 0; s < 7; s++) { pinMode(SEG[s], OUTPUT); digitalWrite(SEG[s], HIGH); }
  pinMode(DP_PIN, OUTPUT); digitalWrite(DP_PIN, HIGH);
  for (int i = 0; i < 4; i++) { pinMode(DIG[i], OUTPUT); digitalWrite(DIG[i], LOW); }
  pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  unsigned long now = millis();

  bool btn = digitalRead(BTN);
  if (prevBtn == HIGH && btn == LOW && now - tBtn > 50) {
    tBtn = now;
    onPress();
  }
  prevBtn = btn;

  if (mode == 1) {
    if (now - tSec >= 1000) {
      tSec = now;
      if (++secs >= 50) { secs = 50; mode = 2; }
    }
    if (now - tDp >= 500) { tDp = now; dpBlink = !dpBlink; }
  } else {
    dpBlink = false;
  }

  if (mode == 0 || mode == 3) {
    showFives();
  } else {
    showNumber(secs, dpBlink);
  }
}