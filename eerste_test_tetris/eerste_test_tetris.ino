#include "LedControl.h"

// LED matrix displays
// Parameters: DIN pin, CLK pin, CS pin
LedControl lc1 = LedControl(13, 11, 12, 4);
LedControl lc2 = LedControl(10, 8, 9, 4);

class Scherm
{
  public:
    void zet_led_aan(int x, int y)
    {
      if (0 <= x && x < 16 && 0 <= y && y < 32)
        new_[y][x] = true;
    }
    void toon()
    {
      for (int y = 0; y < 32; y++)
      {
        for (int x = 0; x < 16; x++)
        {
          if (new_[y][x] != old_[y][x])
          {
            set_led_(x, y, new_[y][x]);
            old_[y][x] = new_[y][x];
          }

          // alle new op uit zetten.
          if (new_[y][x] == true)
          {
            new_[y][x] = false;
          }
        }
      }
    }

  private:
    void set_led_(int x, int y, bool aan)
    {
      bool gebruik_lc1 = (x < 8);

      if (x >= 8)
      {
        x -= 8;
      }

      // Berken a, b en c voor nieuwe LED
      int a = y / 8;
      int rest = y % 8;
      int b = 7 - x;
      int c = 7 - rest;

      if (gebruik_lc1)
      {
        lc1.setLed(a, b, c, aan);
      }
      else
      {
        lc2.setLed(a, b, c, aan);
      }
    }

    bool old_[32][16] = {};
    bool new_[32][16] = {};
};

class Blok
{
  public:
    void zet_positie(int x, int y)
    {
      pos_x = x;
      pos_y = y;
    }
    void zet_steen(int i, int x, int y)
    {
      steen_pos_x[i] = x;
      steen_pos_y[i] = y;
    }

    void naar_links()
    {
      if (pos_x > 0)
        pos_x--;
    }
    void naar_rechts()
    {
      if (pos_x < 15)
        pos_x++;
    }
    void naar_onder()
    {
      if (pos_y > 0)
        pos_y--;
    }
    void draai()
    {
      for (int i = 0; i < 4; ++i)
      {
        int x = steen_pos_x[i];
        int y = steen_pos_y[i];

        // Draai 90 graden naar tegen de klok in
        steen_pos_x[i] = -y;
        steen_pos_y[i] = x;
      }
    }

    bool staat_onderaan() const
    {
      return pos_y == 0;
    }

    void toon_op(Scherm &scherm) const
    {
      for (int i = 0; i < 4; ++i)
      {
        int x = pos_x + steen_pos_x[i];
        int y = pos_y + steen_pos_y[i];
        scherm.zet_led_aan(x, y);
      }
    }

  private:
    int pos_x = 0;
    int pos_y = 0;
    int steen_pos_x[4] = {};
    int steen_pos_y[4] = {};
};

class Spel
{
  public:
    void setup()
    {
      setup_knoppen();
    }

    void loop()
    {
      behandel_blok();

      behandel_knoppen();

      blok.toon_op(scherm);
      scherm.toon();

      if (knop_werd_ingeduwd)
        delay(100);
    }

  private:
    void behandel_blok()
    {
      // Verwijder blok als die helemaal onderaan staat
      if (blok_bestaat)
      {
        if (blok.staat_onderaan())
          blok_bestaat = false;
      }

      // Maak blok aan als die nog niet bestaat
      if (!blok_bestaat)
      {
        maak_random_blok();
        blok.zet_positie(8, 30);
        blok_bestaat = true;
      }

      // Laat blok zakken als het tijd is om hem te laten zakken
      if (millis() >= laat_blok_zakken)
      {
        blok.naar_onder();
        const unsigned long val_snelheid = 200;
        laat_blok_zakken += val_snelheid;
      }
    }

    void behandel_knoppen()
    {
      knop_werd_ingeduwd = false;

      if (digitalRead(knop1_input) == LOW)
      { // Programma voor de pull up weerstand.
        blok.draai();
        knop_werd_ingeduwd = true;
      }

      if (digitalRead(knop3_input) == LOW)
      { // Programma voor de input pull up weerstand.
        blok.naar_rechts();
        knop_werd_ingeduwd = true;
      }

      if (digitalRead(knop2_input) == HIGH)
      { // Programma voor de pull down weerstand.
        blok.naar_links();
        knop_werd_ingeduwd = true;
      }
    }

    void maak_random_blok()
    {
      const long aantal_blokken = 7;
      const long welke_blok = random(aantal_blokken);
      if (welke_blok == 0)
      {
        blok.zet_steen(0, 0, -1);
        blok.zet_steen(1, 0, 0);
        blok.zet_steen(2, 1, 0);
        blok.zet_steen(3, 1, 1);
      }
      else if (welke_blok == 1)
      {
        blok.zet_steen(0, 0, 0);
        blok.zet_steen(1, 0, 1);
        blok.zet_steen(2, 1, 0);
        blok.zet_steen(3, 1, 1);
      }
      else if (welke_blok == 2)
      {
        blok.zet_steen(0, 0, 0);
        blok.zet_steen(1, -1, 0);
        blok.zet_steen(2, 1, 0);
        blok.zet_steen(3, 0, 1);
      }
      else if (welke_blok == 3)
      {
        blok.zet_steen(0, 0, 0);
        blok.zet_steen(1, 0, -1);
        blok.zet_steen(2, 0, 1);
        blok.zet_steen(3, 0, 2);
      }
      else if (welke_blok == 4)
      {
        blok.zet_steen(0, 0, 0);
        blok.zet_steen(1, 0, 1);
        blok.zet_steen(2, 0, -1);
        blok.zet_steen(3, 1, 1);
      }
      else if (welke_blok == 5)
      {
        blok.zet_steen(0, 0, 0);
        blok.zet_steen(1, 0, 1);
        blok.zet_steen(2, -1, 1);
        blok.zet_steen(3, 0, -1);
      }
      else if (welke_blok == 6)
      {
        blok.zet_steen(0, 0, 0);
        blok.zet_steen(1, 0, -1);
        blok.zet_steen(2, -1, 0);
        blok.zet_steen(3, -1, 1);
      }
    }

    void setup_knoppen()
    {
      pinMode(knop1_pullup, OUTPUT);
      pinMode(knop1_input, INPUT);
      pinMode(knop1_low, OUTPUT);
      digitalWrite(knop1_pullup, HIGH);
      digitalWrite(knop1_low, LOW);

      pinMode(knop2_high, OUTPUT);
      pinMode(knop2_input, INPUT);
      pinMode(knop2_low, OUTPUT);
      digitalWrite(knop2_high, HIGH);
      digitalWrite(knop2_low, LOW);

      pinMode(knop3_low, OUTPUT);
      pinMode(knop3_input, INPUT_PULLUP);
      digitalWrite(knop3_low, LOW);
    }

    const int knop1_pullup = A0;
    const int knop1_input = A1;
    const int knop1_low = A2;

    const int knop2_high = 3;
    const int knop2_input = 4;
    const int knop2_low = 5;

    const int knop3_low = 6;
    const int knop3_input = 7;

    Scherm scherm;

    Blok blok;
    bool blok_bestaat = false;

    bool knop_werd_ingeduwd = false;

    // milliseconds wanneer blok moet zakken
    unsigned long laat_blok_zakken = 0;
};

Spel spel;

void setup()
{
  Serial.begin(9600);

  for (int device = 0; device < 4; device++)
  {
    lc1.shutdown(device, false);
    // Set the brightness to a medium value (0 is min, 15 is max)
    lc1.setIntensity(device, 8);
    // Clear the display register
    lc1.clearDisplay(device);

    lc2.shutdown(device, false);
    // Set the brightness to a medium value (0 is min, 15 is max)
    lc2.setIntensity(device, 8);
    // Clear the display register
    lc2.clearDisplay(device);
  }

  spel.setup();
}

void loop()
{
  spel.loop();
}
