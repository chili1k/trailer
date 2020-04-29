#include "Config.h"

void LedButton::setup(LedButtonConfig ledButtonConfig) {
    this->ledButtonConfig = ledButtonConfig;

    button.attach(ledButtonConfig.buttonPin);
    button.interval(BUTTON_DEBOUNCE_INTERVAL);

    pinMode(ledButtonConfig.ledPin, OUTPUT);
}

void LedButton::setOnPressCallback(void (*onPress)(void*), void *objRef) {
    this->onPress = onPress;
    this->onPressObjRef = objRef;
}

void LedButton::setOnReleaseCallback(void (*onRelease)(void*), void *objRef) {
    this->onRelease = onRelease;
    this->onReleaseObjRef = objRef;
}

void LedButton::loop() {
    button.update();

    ButtonState newState = determineState();
    if (newState != buttonState) {
        buttonState = newState;
        notifyObserver(newState);
    }

    if (ledMode == LedMode::BLINKING)
        blinkLed();
}

void LedButton::blinkLed() {
    if (blinkLedTimer.elapsedMs() >= (unsigned long) blinkIntervalMs) {
        // Toggle led on interval
        doSetLed(!isLedOn);
        blinkLedTimer.restart();
    }
}

void LedButton::notifyObserver(ButtonState newState) {
    switch (newState) {
        case ButtonState::PRESS:
            if (onPress != NULL)
                onPress(onPressObjRef);
            break;
        case ButtonState::RELEASE:
            if (onRelease != NULL)
                onRelease(onReleaseObjRef);
            break;
        case ButtonState::NONE:
            break;
    }
}

ButtonState LedButton::determineState() {
    if (button.read()) {
        if (!pressedButtonTimer.isStarted()) {
            pressedButtonTimer.start();
        }
        return ButtonState::PRESS;
    }
    else if (button.fell()) {
        pressedButtonTimer.stop();
        return ButtonState::RELEASE;
    }

    pressedButtonTimer.stop();

    return ButtonState::NONE;
}

void LedButton::setLedOn() {
    ledMode = LedMode::ON;
    doSetLed(true);
}

void LedButton::setLedOff() {
    ledMode = LedMode::OFF;
    doSetLed(false);
}

void LedButton::setLedBlinking(int intervalMs) {
    blinkIntervalMs = intervalMs;
    ledMode = LedMode::BLINKING;

    // Start with led ON
    doSetLed(true);
    blinkLedTimer.restart();
}

void LedButton::doSetLed(bool isOn) {
    isLedOn = isOn;
    digitalWrite(ledButtonConfig.ledPin, isOn);
}

unsigned long LedButton::getPressedButtonDurationMs() {
    if (buttonState == ButtonState::PRESS || buttonState == ButtonState::RELEASE) {
        return pressedButtonTimer.elapsedMs();
    }

    return 0;
}

bool LedButton::isPressed() {
    return buttonState == ButtonState::PRESS;
}

