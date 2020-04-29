#ifndef TrailerController_h
#define TrailerController_h

#include <Bounce2.h>
#include "GyroQuartiles.h"
#include "ViewObserver.h"

#include "Leg.h"
#include "Balancer.h"
#include "SerialView.h"
#include "ButtonView.h"
#include "util/Timer.h"

class TrailerController : public ViewObserver {
    public:
        void setup();
        void loop();
        void onCommand(const CommandResult commandResult);

    private:
        void handleCommandResult(CommandResult commandResult);
        void startSingleMotor(int motorId, int direction);
        void setPWM(int legId, int direction);
        void handleCommand(CommandResult commandResult);

        void blinkLed();
        void handleEmergencyButton();

        Balancer balancer;
        Timer startTimer;
        Timer blinkLedTimer;
        Bounce emergencyButton;
        GyroQuartiles gyroQuartiles;
        SerialView serialView;
        ButtonView buttonView;
};

#endif
