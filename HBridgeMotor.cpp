#include "HBridgeMotor.h"
#include "Arduino.h"

HBridgeMotor::HBridgeMotor(int motorType) {
    this->hBConfig = hBConfigs[motorType];
}

void HBridgeMotor::setup(MotorConfig motorConfig) {
    this->motorConfig = motorConfig;
    pinMode(motorConfig.directionPin1, OUTPUT);
    pinMode(motorConfig.enablePin, OUTPUT);
    this->pwm = motorConfig.pwm;

    if (hBConfig.pinMode == TWO_PIN)
        pinMode(motorConfig.directionPin2, OUTPUT);
}

void HBridgeMotor::writeBits(short bits) {
    digitalWrite(motorConfig.directionPin1, bits & 0x1);

    if (hBConfig.pinMode == TWO_PIN)
        digitalWrite(motorConfig.directionPin2, bits >> 1 & 0x1);
}

void HBridgeMotor::enable(bool on) {
    int power = on ? pwm : 0;
    analogWrite(motorConfig.enablePin, power);
}

void HBridgeMotor::forward() {
    brake();

    writeBits(hBConfig.bitsForward);
    enable(true);
}

void HBridgeMotor::reverse() {
    brake();

    writeBits(hBConfig.bitsReverse);
    enable(true);
}

void HBridgeMotor::brake() {
    // Always set PWM to 0 when stopping
    enable(false);
    writeBits(hBConfig.bitsBrake);
}

void HBridgeMotor::stop() {
    // Always set PWM to 0 when stopping
    digitalWrite(motorConfig.enablePin, 0);
    writeBits(hBConfig.bitsStop);
}

void HBridgeMotor::setPWM(int pwm) {
    this->pwm = pwm;
}
