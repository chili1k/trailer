#ifndef TrailerController_h
#define TrailerController_h

#include <Bounce2.h>
#include "GyroQuartiles.h"

#include "Leg.h"
#include "Balancer.h"
#include "SerialDebugWriter.h"
#include "util/Timer.h"

/**
 Controls trailer based on user input.
 */
class TrailerController {
    public:
        TrailerController():
            timestamp(0),
            lastRefreshTime(0) {}

        void setup();
        void loop();

    private:
        void handleInput();
        void handleEmergencyButton();
        void startSingleMotor();
        void setPWM();
        void blinkLed();
        void refreshDisplay();
        void refreshSerialDebug();
        void printHeader();
        void printTrailerState();
        void printLeg(Leg *leg);
        void printGyro();
        void printAmpers(Leg *leg);
        void printLegPosition(Leg *leg);
        void printCommands();
        void printTitle();

        Balancer balancer;
        Timer startTimer;
        Timer serialDebugTimer;
        unsigned long timestamp;
        unsigned long lastRefreshTime;
        Bounce emergencyButton;
        GyroQuartiles gyroQuartiles;
        SerialDebugWriter serialDebugWriter;
};

#endif
