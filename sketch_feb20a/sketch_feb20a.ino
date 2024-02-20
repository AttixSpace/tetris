#include "LedControl.h"

// Parameters: DIN pin, CLK pin, CS pin, number of cascaded MAX7219 devices
LedControl lc = LedControl(11, 13, 10, 4);

void setup() {
  // Wake up the MAX7219 from power-saving mode
  for (int device = 0; device < 4; device++) {
    lc.shutdown(device, false);
    // Set the brightness to a medium value (0 is min, 15 is max)
    lc.setIntensity(device, 8);
    // Clear the display register
    lc.clearDisplay(device);
  }

  // Light up a single LED
  // Parameters: (device number, row, column, state)
  // Note: Adjust the device number (0-3) based on which 8x8 segment the LED is in
  // Rows and columns are 0-indexed
  lc.setLed(0, 0, 0, true); // This will light up the top-left LED in the first 8x8 segment
}

void loop() {
  // No need to do anything here
}
