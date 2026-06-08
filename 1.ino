int digit[4] = {28, 27, 22, 14};
int number[10] = {126, 12, 182, 158, 204, 218, 250, 14, 254, 222};

int d;

unsigned long t0 = 0;
unsigned long pointTimer = 0;

int t = 0;

int dig1;
int dig2;
int dig3;
int dig4;

bool pointState = false;

// 0 - сброс
// 1 - счет
// 2 - стоп
byte mode = 0;

void setup() {

  DDRD = 254;   // D1-D7 выходы
  DDRB = 31;    // D8-D12 выходы, D13 вход

  PORTB = 30;   // выключить все разряды
}

void loop() {

  if (digitalRead(13) == 1) {

    delay(200);

    if (mode == 0) {
      mode = 1;
      t0 = millis();
    }
    else if (mode == 1) {
      mode = 2;
    }
    else {
      mode = 0;
      t = 0;
    }

    while (digitalRead(13) == 1);
  }

  if (mode == 1) {

    t = (millis() - t0) / 1000;

    if (t >= 60) {
      t = 60;
      mode = 2;
    }
  }

  if (millis() - pointTimer >= 500) {
    pointTimer = millis();
    pointState = !pointState;
  }

  out(t);
}

void out(int value) {

  dig1 = value / 1000;
  dig2 = (value % 1000) / 100;
  dig3 = (value % 100) / 10;
  dig4 = value % 10;

  for (d = 0; d <= 3; d++) {

    PORTB = digit[d];

    switch (d) {

      case 0:
        PORTD = number[dig1];
        break;

      case 1:
        PORTD = number[dig2];
        break;

      case 2:
        if (pointState)
          PORTD = number[dig3] + 1;
        else
          PORTD = number[dig3];
        break;

      case 3:
        PORTD = number[dig4];
        break;
    }

    delay(5);
  }
}