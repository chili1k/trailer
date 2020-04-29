#include "WinchMotor.h"
#include "Arduino.h"

#define MOTOR_FORWARD_STATE false
#define MOTOR_REVERSE_STATE !MOTOR_FORWARD_STATE
// Pin configurations for motor ON and OFF.
#define MOTOR_ON_PIN_MODE false
#define MOTOR_OFF_PIN_MODE !MOTOR_ON_PIN_MODE

void WinchMotor::setup(MotorConfig motorConfig) {
    this->directionPin = motorConfig.directionPin1;
    this->powerPin = motorConfig.enablePin;

    pinMode(directionPin, OUTPUT);
    pinMode(powerPin, OUTPUT);
    digitalWrite(directionPin, MOTOR_OFF_PIN_MODE);
    digitalWrite(powerPin, MOTOR_OFF_PIN_MODE);
}


void WinchMotor::forward() {
    digitalWrite(directionPin, MOTOR_FORWARD_STATE);
    digitalWrite(powerPin, MOTOR_ON_PIN_MODE);
}

void WinchMotor::reverse() {
    digitalWrite(directionPin, MOTOR_REVERSE_STATE);
    digitalWrite(powerPin, MOTOR_ON_PIN_MODE);
}

void WinchMotor::brake() {
    stop();
}

void WinchMotor::stop() {
    digitalWrite(powerPin, MOTOR_OFF_PIN_MODE);
    digitalWrite(directionPin, MOTOR_OFF_PIN_MODE);
}

// Not used
void WinchMotor::setPWM(int pwm) {
}
