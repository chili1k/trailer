#include "Display.h"
#include "Arduino.h"

void Display::setup() {
  Serial.begin(115200); 
}

void Display::print(const char *message) {
  Serial.print(message);  
}

void Display::println(const char *message) {
  Serial.println(message);  
}

