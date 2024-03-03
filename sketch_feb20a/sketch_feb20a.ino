#include "LedControl.h"



// Parameters: DIN pin, CLK pin, CS pin, number of cascaded MAX7219 devices
LedControl lc = LedControl(11, 13, 10, 4);
LedControl lc2 = LedControl(7, 5, 6, 4);

int a = 0;
int b = 0;
int c = 0;

void led(int x, int y){
  lc.setLed(a,b,c, false);
  a = y/8;
  int rest = y%8;
  b = 7-x;
  c = 7-rest;
  lc.setLed(a,b,c,true);
}

void setup() {
  // Wake up the MAX7219 from power-saving mode
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
  

  // Light up a single LED
  // Parameters: (device number, row, column, state)
  // Note: Adjust the device number (0-3) based on which 8x8 segment the LED is in
  // Rows and columns are 0-indexed
  

}
void loop() {
   for(int y = 0; y<32; y++){
    led(y%8, y);
    delay(1000);  
  }
}
