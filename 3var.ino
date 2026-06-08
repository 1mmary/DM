int digit[4] = {28,27,22,14};      // переключение разрядов индикатора
int number[10] = {126,12,182,158,204,218,250,14,254,222};

int d;
int n = 0;

int t0;     // момент старта
int t;      // текущее время

int dig1;
int dig2;
int dig3;
int dig4;

bool start = false;

void setup()
{
    DDRD = 254;      // D1-D7 выходы
    DDRB = 31;       // D8-D12 выходы, D13 вход
    PORTB = 30;      // отключаем все разряды
}

void loop()
{
    if (digitalRead(13) == 1)
    {
        start = !start;
        t0 = millis();
        delay(200);
    }

    if (start == true)
    {
        t = (millis() - t0) / 10;   // сотые доли секунды
        out(t);
    }
    else
    {
        out(t);
    }
}

void out(int value)
{
    dig1 = value / 1000;
    dig2 = (value % 1000) / 100;
    dig3 = ((value % 1000) % 100) / 10;
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