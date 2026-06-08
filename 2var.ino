// Секундомер 0-90 с кнопкой и индикацией 9999 при старте
// Порт D (пины 1-7) – сегменты, порт B (пины 8-12) – выбор разрядов
// Кнопка на пине 13 (подтяжка к питанию)

int digi[4] = {28, 27, 22, 14};        // маски выбора разрядов для PORTB
int number[10] = {126, 12, 182, 158, 204, 218, 250, 14, 254, 222}; // коды цифр для PORTD

int counter = 0;          // текущее значение 0..90
int mode = 0;             // 0 – счёт (RUN), 1 – пауза (PAUSE)
bool show9999 = true;     // флаг отображения 9999 при старте
bool dotBlink = false;    // флаг мигания точки
unsigned long lastBlink = 0;
unsigned long lastSecond = 0;
bool lastBtnState = HIGH;

void setup() {
  DDRD = 254;              // PD1-PD7 выходы (сегменты), PD0 не используется
  DDRB = 31;               // PB0-PB4 выходы (выбор разрядов)
  pinMode(13, INPUT_PULLUP); // кнопка на пине 13, подтяжка к +5V
  PORTB = 0;               // все разряды выключены
  PORTD = 0;               // все сегменты выключены
}

// Функция вывода на 4-разрядный индикатор
void display(int value) {
  int d1, d2, d3, d4;
  if (value == 9999) {
    d1 = d2 = d3 = d4 = 9;
  } else {
    if (value > 90) value = 90;
    d1 = 0;                // первый разряд (тысячи) – 0
    d2 = 0;                // второй разряд (сотни) – 0
    d3 = value / 10;       // десятки
    d4 = value % 10;       // единицы
  }
  for (int d = 0; d < 4; d++) {
    PORTB = digi[d];       // включаем текущий разряд
    int digitVal;
    switch (d) {
      case 0: digitVal = d1; break;
      case 1: digitVal = d2; break;
      case 2: digitVal = d3; break;
      case 3: digitVal = d4; break;
    }
    byte seg = number[digitVal];
    if (dotBlink) seg |= 0x80;   // зажигаем десятичную точку (бит 7)
    PORTD = seg;
    delay(5);                     // задержка динамической индикации
  }
}

void loop() {
  // --- обработка кнопки (антидребезг) ---
  bool btn = digitalRead(13);
  if (btn == LOW && lastBtnState == HIGH) {
    delay(50);   // подавление дребезга
    if (show9999) {
      // первое нажатие – убираем 9999, запускаем счёт с 0
      show9999 = false;
      counter = 0;
      mode = 0;
      lastSecond = millis();
    } else {
      // переключение режимов: RUN <-> PAUSE, в PAUSE – сброс
      if (mode == 0) {
        mode = 1;   // RUN -> PAUSE
      } else if (mode == 1) {
        counter = 0;  // PAUSE -> сброс на 0 (остаёмся в PAUSE)
      }
    }
  }
  lastBtnState = btn;

  // --- логика счёта (только если не 9999 и режим RUN) ---
  if (!show9999 && mode == 0) {
    unsigned long now = millis();
    if (now - lastSecond >= 1000) {
      lastSecond = now;
      if (counter < 90) {
        counter++;
        dotBlink = true;      // включаем мигание точки
        lastBlink = now;
      }
      if (counter == 90) {
        mode = 1;             // достигли 90 – останов
      }
    }
  }

  // --- управление миганием точки (длительность 100 мс) ---
  if (dotBlink && (millis() - lastBlink >= 100)) {
    dotBlink = false;
  }

  // --- вывод на индикатор ---
  if (show9999) display(9999);
  else display(counter);
}