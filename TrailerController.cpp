#include <Arduino.h>
#include <Bounce2.h>
#include <avr/wdt.h>

#include "TrailerController.h"
#include "Leg.h"
#include "Gyro.h"
#include "Balancer.h"
#include "GyroQuartiles.h"
#include "Config.h"
#include "Debug.h"

#define STATS_DELAY_MS 5000

bool wasEmergencyButtonPushed;
bool blinkLedState;
bool canStartStats = false;

void TrailerController::setup() {
    balancer.setup(&gyroQuartiles, &serialView, &buttonView);

    gyroQuartiles.setup(balancer.getGyro());

    serialView.setup(&balancer, &gyroQuartiles);
    serialView.registerViewObserver(this);
    buttonView.setup(&balancer, &gyroQuartiles);
    buttonView.registerViewObserver(this);

    pinMode(BLINK_LED_PIN, OUTPUT);
    pinMode(EMERGENCY_STOP_PIN, INPUT);

    emergencyButton.attach(EMERGENCY_STOP_PIN);
    emergencyButton.interval(BUTTON_DEBOUNCE_INTERVAL);

    startTimer.restart();
    blinkLedTimer.restart();
}

void TrailerController::loop() {
    handleEmergencyButton();
    balancer.loop();

    serialView.loop();
    buttonView.loop();

    if (startTimer.elapsedMs() > STATS_DELAY_MS)
        canStartStats = true;

    if (canStartStats) {
        gyroQuartiles.loop();
    }

    blinkLed();
}

void TrailerController::onCommand(const CommandResult commandResult) {
    switch (commandResult.command) {
        case ViewCommand::NONE:
            break;
        case ViewCommand::STOP_ALL:
            balancer.stopAllLegs();
            break;
        case ViewCommand::BALANCE:
            balancer.balance();
            break;
        case ViewCommand::TO_ZERO:
            balancer.toZero();
            break;
        case ViewCommand::TO_GROUND:
            balancer.toGround();
            break;
        case ViewCommand::TO_FINAL:
            balancer.toFinal();
            break;
        case ViewCommand::START_SINGLE_MOTOR:
            startSingleMotor(commandResult.legId, commandResult.direction);
            break;
        case ViewCommand::SET_PWM:
            setPWM(commandResult.legId, commandResult.direction);
            break;
    }
}

void TrailerController::blinkLed() {
    if (blinkLedTimer.elapsedMs() < BLINK_LED_INTERVAL_MS) {
        return;
    }

    blinkLedTimer.restart();

    blinkLedState = !blinkLedState;
    digitalWrite(BLINK_LED_PIN, blinkLedState);
}

void TrailerController::handleEmergencyButton() {
    emergencyButton.update();

    bool pressDetected = emergencyButton.read();
    if (!wasEmergencyButtonPushed) {
        if (pressDetected) {
            balancer.stopAllLegs();
            wasEmergencyButtonPushed = true;
        }
    } else {
        if (!pressDetected) {
            wasEmergencyButtonPushed = false;
        }
    }
}

void TrailerController::setPWM(int motorId, int pwm) {
    balancer.forcePWM(motorId, pwm);
}

void TrailerController::startSingleMotor(int motorId, int direction) {
    if (direction == 0) {
        balancer.forceExpandLeg(motorId);
    } else {
        balancer.forceCollapseLeg(motorId);
    }
}

