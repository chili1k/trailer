#include "Gyro.h"

Gyro gyro;

void setup() {
//  gyro.setup();
  Serial.begin(115200);
}

void loop() {
//  gyro.loop();
  Serial.println("Bla");
  delay(1000);
}
