int digit[4] = {28, 27, 22, 14};
int number[10] = {126, 12, 182, 158, 204, 218, 250, 14, 254, 222};

int d;

unsigned long startTime = 0;
int secondsCount = 0;

int dig1;
int dig2;
int dig3;
int dig4;

byte mode = 0;   // 0 - сброс, 1 - счёт, 2 - стоп

void setup()
{
  DDRD = 254;    // D1-D7 выходы
  DDRB = 31;     // D8-D12 выходы, D13 вход

  PORTB = 30;    // выключить все разряды

  pinMode(13, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(13) == LOW)
  {
    delay(200);

    while (digitalRead(13) == LOW);

    switch (mode)
    {
      case 0:                 // Сброс -> Старт
        mode = 1;
        startTime = millis();
        break;

      case 1:                 // Старт -> Стоп
        mode = 2;
        break;

      case 2:                 // Стоп -> Сброс
        mode = 0;
        secondsCount = 0;
        break;
    }
  }

  if (mode == 1)
  {
    secondsCount = (millis() - startTime) / 1000;

    if (secondsCount >= 60)
    {
      secondsCount = 60;
      mode = 2;
    }
  }

  out(secondsCount);
}

void out(int value)
{
  dig1 = value / 1000;
  dig2 = (value % 1000) / 100;
  dig3 = (value % 100) / 10;
  dig4 = value % 10;

  for (d = 0; d <= 3; d++)
  {
    PORTB = digit[d];

    switch (d)
    {
      case 0:
        PORTD = number[dig1];
        break;

      case 1:
        PORTD = number[dig2];
        break;

      case 2:
        PORTD = number[dig3];
        break;

      case 3:
        PORTD = number[dig4];
        break;
    }

    delay(5);
  }
}