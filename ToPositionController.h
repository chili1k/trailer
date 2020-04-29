#ifndef ToPositionController_h
#define ToPositionController_h

#include "Leg.h"
#include "BalancerState.h"
#include "Gyro.h"

enum class ToPositionResult { Error, Moving, Done };

class ToPositionController {
    public:
        void setup(Leg *legs);
        void reset(State desiredState);
        ToPositionResult loop();

    private:
        void moveToFinalOrZeroLoop();
        void nextPosition();

        Leg *legs;
        State desiredState;
        LegPosition desiredLegState;
        unsigned long lastToPositionChange;
        int toPositionCurrentPos;
        int positionsOnEndState;
};

#endif
