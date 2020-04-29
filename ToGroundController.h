#ifndef ToGroundController_h
#define ToGroundController_h

#include "Leg.h"
#include "BalancerState.h"
#include "Gyro.h"
#include "GyroQuartiles.h"

enum class ToGroundResult { Error, Moving, Done };
enum class ToGroundState { Error, None, MovingTwoLegs, MovingSingleLeg, Done };

class ToGroundController {
    public:
        void setup(Leg *legs, GyroQuartiles *gyroQuartiles);
        void start();
        ToGroundResult loop();

    private:
        void moveToGroundLoop();
        void nextPosition();
        void determineGround();
        void moveTwoLegsLoop();
        void moveSingleLegLoop();

        Leg *legs;
        GyroQuartiles *gyroQuartiles;

        ToGroundState state;
        int toPositionCurrentPos;
        int lastToPositionChange;

        // Used to determine how long trailer has been in ground levels
        unsigned long groundLevelTimer;
        bool isLegPairWithinGroundLevels;

};

#endif
