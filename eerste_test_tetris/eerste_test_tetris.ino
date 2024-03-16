// C++ code
//
#include "LedControl.h"
// Parameters: DIN pin, CLK pin, CS pin
LedControl lc1 = LedControl(13, 11, 12, 4);
LedControl lc2 = LedControl(10, 8, 9, 4);


class Led
{
  public:
    void led(int x, int y) {
      // Zet vorige LED uit
      lc1.setLed(a, b, c, false);

      // Berken a, b en c voor nieuwe LED
      a = y / 8;
      int rest = y % 8;
      b = 7 - x;
      c = 7 - rest;

      // Zet nieuwe LED aan
      lc1.setLed(a, b, c, true);
    }

  private:
    int a = 0;
    int b = 0;
    int c = 0;
};
Led l1;
Led l2;
Led l3;

class Board
{
  public:
    void zet_led_aan(int x, int y) {
      new_[y][x] = true;
    }
    void show() {
      for (int y  = 0; y < 32; y++) {
        for (int x = 0; x < 16; x++) {
          if (new_[y][x] != old_[y][x]) {
            set_led_(x, y, new_[y][x]);
            old_[y][x] = new_[y][x];
          }

          //alle new op uit zetten.
          if (new_[y][x] == true) {
            new_[y][x] = false;
          }
        }
      }
    }
  private:

    void set_led_(int x, int y, bool aan) {
      bool gebruik_lc1 = (x < 8);

      if (x >= 8) {
        x -= 8;
      }

      // Berken a, b en c voor nieuwe LED
      int a = y / 8;
      int rest = y % 8;
      int b = 7 - x;
      int c = 7 - rest;

      if (gebruik_lc1) {
        lc1.setLed(a, b, c, aan);
      }
      else {
        lc2.setLed(a, b, c, aan);
      }
    }

    bool old_[32][16] = {};
    bool new_[32][16] = {};
};

Board b;


const int knop1_pullup = A0;
const int knop1_input = A1;
const int knop1_low = A2;

const int knop2_high = 3;
const int knop2_input = 4;
const int knop2_low = 5;

const int knop3_low = 6;
const int knop3_input = 7;

void setup()
{
  for (int device = 0; device < 4; device++) {
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

  Serial.begin(9600);

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

void loop()
{
  {
    const int knopwaarde = digitalRead(knop1_input);
    if (knopwaarde == LOW) { //Programma voor de pull up weerstand.
      Serial.println("De knop2 is ingedrukt.");
      b.zet_led_aan(2, 4);
      b.show();
    }
  }

  {
    const int knopwaarde = digitalRead(knop2_input);
    if (knopwaarde == HIGH) { //Programma voor de pull up weerstand.
      Serial.println("De knop2 is ingedrukt.");
      b.zet_led_aan(7, 17);
      b.show();
    }
    else {
      Serial.println("De knop2 is niet ingedrukt.");
    }
  }

  {
    const int knopwaarde = digitalRead(knop3_input);
    if (knopwaarde == LOW) { //Programma voor de pull up weerstand.
      Serial.println("De knop2 is ingedrukt.");
      b.zet_led_aan(15, 27);
      b.zet_led_aan(8, 27);
      b.show();
    }
    else {
      Serial.println("De knop2 is niet ingedrukt.");
    }
  }
}


void loop_old()
{
  {
    const int knopwaarde = digitalRead(knop1_input);
    if (knopwaarde == LOW)
    {
      //Programma voor de pull down weerstand.
      Serial.println("De knop1 is ingedrukt.");
      for (int y = 0; y < 32; y++) {
        l2.led(y % 8, y);
        delay(10);
      }
    }
    else {
      Serial.println("De knop1 is niet ingedrukt.");
    }
  }

  {
    const int knopwaarde = digitalRead(knop2_input);
    if (knopwaarde == HIGH) { //Programma voor de pull up weerstand.
      Serial.println("De knop2 is ingedrukt.");
      for (int x = 0; x < 32; x++) {
        l3.led(0, x);
        delay(20);
      }
    }
    else {
      Serial.println("De knop2 is niet ingedrukt.");
    }
  }

  {
    const int knopwaarde = digitalRead(knop3_input);
    if (knopwaarde == LOW) { //Programma voor de input pull up weerstand.
      Serial.println("De knop3 is ingedrukt.");
      for (int z = 0; z < 8; z++) {
        l1.led(z % 8, 0);
        delay(20);
      }
    }
    else {
      Serial.println("De knop3 is niet ingedrukt.");
    }
  }
}
