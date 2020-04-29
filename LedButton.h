#ifndef LedButton_h
#define LedButton_h

#include <Bounce2.h>
#include "util/Timer.h"

enum class LedMode { ON, OFF, BLINKING };
enum class ButtonState { NONE, PRESS, RELEASE };

struct LedButtonConfig {
    int buttonPin;
    int ledPin;
};

class LedButton {
    public:
        void setup(LedButtonConfig buttonConfig);
        void setOnPressCallback(void (*onPress)(void*), void *objRef);
        void setOnReleaseCallback(void (*onRelease)(void*), void *objRef);
        void loop();

        void setLedOn();
        void setLedOff();
        void setLedBlinking(int intervalMs);

        unsigned long getPressedButtonDurationMs();
        bool isPressed();
    private:
        ButtonState determineState();
        void notifyObserver(ButtonState state);
        void blinkLed();
        void doSetLed(bool isOn);

        void (*onPress)(void*) = NULL;
        void (*onRelease)(void*) = NULL;
        void *onPressObjRef = NULL;
        void *onReleaseObjRef = NULL;
        ButtonState buttonState = ButtonState::NONE;
        LedButtonConfig ledButtonConfig;
        int blinkIntervalMs;
        Bounce button;
        LedMode ledMode = LedMode::OFF;
        Timer blinkLedTimer;
        Timer pressedButtonTimer;
        bool isLedOn = false;
};

#endif
