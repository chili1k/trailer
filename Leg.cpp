#include <math.h>

#include "Arduino.h"
#include <Bounce2.h>

#include "Leg.h"
#include "Debug.h"
#include "Config.h"

#define CURRENT_DELTA_INTERVAL_MS 1000
#define LEG_GROUND_DELTA_AMPS 0.4
#define LEG_GROUND_MIN_AMPS 5.0
#define LEG_POS_DEBOUNCE_MS 20

#define FILTER_FREQUENCY 10.0

int lastAmpReadTime = 0;
float previousAmps = 0.0;

Leg::Leg(LegConfig legConfig) {
    this->legConfig = legConfig;
    this->motorConfig.directionPin1 = legConfig.pinMotorDirection1;
    this->motorConfig.directionPin2 = legConfig.pinMotorDirection2;
    this->motorConfig.enablePin = legConfig.pinMotorPower;
    this->motorConfig.pwm = legConfig.pwm;
    this->motor = legConfig.motor;
    this->motorState = MotorState::Stopped;
}

void Leg::setup() {
    motor->setup(motorConfig);

    pinMode(legConfig.pinZeroPos, INPUT);
    debounceZero.attach(legConfig.pinZeroPos);
    debounceZero.interval(LEG_POS_DEBOUNCE_MS);

    pinMode(legConfig.pinFinalPos, INPUT);
    debounceFinal.attach(legConfig.pinFinalPos);
    debounceFinal.interval(LEG_POS_DEBOUNCE_MS);

    pinMode(legConfig.pinPowerMeter, INPUT);
}

const char *Leg::getName() {
    return legConfig.name;
}

void Leg::loop() {
    debounceZero.update();
    debounceFinal.update();

    loopCheckIfOnGround();

    //  int powerReading = lowpassFilter.input(analogRead(legConfig.pinPowerMeter));
    int powerReading = analogRead(legConfig.pinPowerMeter);
    smoother.putReading(powerReading);

    // Safety - do not allow leg over final or zero position
    if (motorState == MotorState::Forward && getPosition() == LegPosition::Final) {
        stop();
    }

    if (motorState == MotorState::Reverse && getPosition() == LegPosition::Zero) {
        stop();
    }
}

void Leg::expand() {
    // We can only reset isOnGround to false only when leg reaches zero position.
    if (getPosition() == LegPosition::Zero) {
        previousAmps = 0.0;
        _isOnGround = false;
    }

    if (getPosition() == LegPosition::Final) {
        DPRINTLN(F("Cannot expand leg beyond final position."));
        return;
    }

    motor->forward();
    motorState = MotorState::Forward;
}

void Leg::stop() {
    previousAmps = 0.0;
    motor->brake();
    motorState = MotorState::Stopped;
}

void Leg::setPWM(int pwm) {
    motorConfig.pwm = pwm;
    motor->setPWM(pwm);
}

void Leg::collapse() {
    if (getPosition() == LegPosition::Zero) {
        DPRINTLN(F("Cannot collapse leg beyond zero position."));
        return;
    }

    motor->reverse();
    motorState = MotorState::Reverse;
}

LegPosition Leg::getPosition() {
    if (debounceZero.read() == HIGH) {
        return LegPosition::Zero;
    }

    if (debounceFinal.read() == HIGH) {
        return LegPosition::Final;
    }

    if (motorState == MotorState::Forward) {
        return LegPosition::Expanding;
    }

    if (motorState == MotorState::Reverse) {
        return LegPosition::Collapsing;
    }

    return LegPosition::Unknown;
}

int Leg::getPWM() {
    return motorConfig.pwm;
}

bool Leg::isOnGround() {
    return _isOnGround;
}

bool Leg::isMotorRunning() {
    return motorState != MotorState::Stopped;
}

bool Leg::isMotorStopped() {
    return !isMotorRunning();
}

float Leg::getAmpers() {
    //float mV = (raw / 1023.0) * 5000.0;
    //float amps = ((2500.0 - mV) / 66.0);

    //  float rawAverage = smoother.getAverage();
    //  return 37.87-(0.07405 * rawAverage);

    float raw = smoother.getAverage();
    // 43400
    // For some reason voltage is at 300 mV offset
    float mV = (raw / 1023.0) * (5000.0-450);
    return ((2500.0 - mV) / 66.0);
}

void Leg::loopCheckIfOnGround() {
    /*
    if (_isOnGround || isMotorStopped() || motor.getDirection() != MOTOR_DIRECTION_EXPANDING) {
        return;
    }

    unsigned long now = millis();

    // Do not read amps too often.
    if ((now - lastAmpReadTime) < CURRENT_DELTA_INTERVAL_MS) {
        return;
    }

    lastAmpReadTime = now;

    // We don't care how the current meter is turned, because
    // power is only high when leg is expanding
    float ampsNow = abs(getAmpers());

    // Do not check on first reading.
    if (previousAmps > 0) {
        _isOnGround = (ampsNow - previousAmps) >= LEG_GROUND_DELTA_AMPS;
    } else {
        _isOnGround = false;
    }

    if (ampsNow > LEG_GROUND_MIN_AMPS) {
        //    DPRINT("Saving amps");
        //    DPRINTLN(ampsNow);
        previousAmps = ampsNow;
    }
    */
}


bool Leg::isHighAmperage() {
    return _isHighAmperage;
}

void Leg::legStateToString(char *&state) {
    switch (getPosition()) {
        case (LegPosition::Unknown):
            state = (char *)"Unknown";
            break;
        case (LegPosition::Expanding):
            state = (char *)"Expanding";
            break;
        case (LegPosition::Collapsing):
            state = (char *)"Collapsing";
            break;
        case (LegPosition::Zero):
            state = (char *)"Zero";
            break;
        case (LegPosition::Final):
            state = (char *)"Final";
            break;
        default:
            state = (char *)"ERROR STATE";
    }
}

