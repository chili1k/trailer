#ifndef SerialView_h
#define SerialView_h

#include "BalancerState.h"
#include "View.h"
#include "SerialDebugWriter.h"
#include "Leg.h"
#include "util/Timer.h"

class SerialView : public View {
    public:
        SerialView():
            timestamp(0),
            lastRefreshTime(0) {}
        void setup(Balancer *balancer, GyroQuartiles *gyroQuartiles);
        void loop();

        void registerViewObserver(ViewObserver *viewObserver);
        void notifyStateChange(State newState);


    private:
        void readInput();
        void refreshDisplay();

        void printHeader();
        void printTrailerState();
        void printLeg(Leg *leg);
        void printGyro();
        void printAmpers(Leg *leg);
        void printLegPosition(Leg *leg);
        void printCommands();
        void printTitle();
        void readMotorInput(CommandResult *commandResult);
        void readPWM(CommandResult &commandResult);
        int readMotor();
        void refreshSerialDebug();

        SerialDebugWriter serialDebugWriter;
        Balancer *balancer;
        Leg *legs;
        Gyro *gyro;
        GyroQuartiles *gyroQuartiles;
        unsigned long timestamp;
        unsigned long lastRefreshTime;
        Timer serialDebugTimer;
        ViewObserver *viewObserver = NULL;
};

#endif
