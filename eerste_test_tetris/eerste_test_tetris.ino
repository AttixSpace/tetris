// C++ code
//
#include "LedControl.h"
LedControl lc = LedControl(11, 13, 10, 4);
LedControl lc2 = LedControl(7, 5, 6, 4);

int a = 0;
int b = 0;
int c = 0;

void led(int x, int y) {
  // Zet vorige LED uit
  lc.setLed(a, b, c, false);

  // Berken a, b en c voor nieuwe LED
  a = y / 8;
  int rest = y % 8;
  b = 7 - x;
  c = 7 - rest;

  // Zet nieuwe LED aan
  lc.setLed(a, b, c, true);
}

const int knop1_pullup = A0;
const int knop1_input = A1;
const int knop1_low = A2;

const int k23 = 3;
const int k24 = 4;
const int k25 = 5;

const int k36 = 6;
const int k37 = 7;

void setup()
{
  for (int device = 0; device < 4; device++) {
    lc.shutdown(device, false);
    // Set the brightness to a medium value (0 is min, 15 is max)
    lc.setIntensity(device, 8);
    // Clear the display register
    lc.clearDisplay(device);

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

  pinMode(k23, OUTPUT);
  pinMode(k24, INPUT);
  pinMode(k25, OUTPUT);
  digitalWrite(k23, HIGH);
  digitalWrite(k25, LOW);

  pinMode(k36, OUTPUT);
  pinMode(k37, INPUT_PULLUP);
  digitalWrite(k36, LOW);
}

void loop()
{
  {
    const int knopwaarde = digitalRead(knop1_input);
    if (knopwaarde == LOW)
    {
      //Programma voor de pull down weerstand.
      Serial.println("De knop1 is ingedrukt.");
      for (int y = 0; y < 32; y++) {
        led(y % 8, y);
        delay(10);
      }
    }
    else {
      Serial.println("De knop1 is niet ingedrukt.");
    }
  }

  {
    const int knopwaarde = digitalRead(k24);
    if (knopwaarde == HIGH) { //Programma voor de pull up weerstand.
      Serial.println("De knop2 is ingedrukt.");
      for (int x = 0; x < 32; x++) {
        led(0, x);
        delay(20);
      }
    }
    else {
      Serial.println("De knop2 is niet ingedrukt.");
    }
  }

  {
    const int knopwaarde = digitalRead(k37);
    if (knopwaarde == LOW) { //Programma voor de input pull up weerstand.
      Serial.println("De knop3 is ingedrukt.");
      for (int z = 0; z < 8; z++) {
        led(z % 8, 0);
        delay(20);
      }
    }
    else {
      Serial.println("De knop3 is niet ingedrukt.");
    }
  }
}
