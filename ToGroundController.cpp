/**
 * Moves legs to ground:
 * - First move two legs in parallel
 * - When stdev is high enough start moving individual legs
 * - Each leg must have stdev high enoug
 */

#include "Config.h"
#include "ToGroundController.h"
#include "LegUtil.h"
#include "Leg.h"
#include "Debug.h"

#define TO_POSITION_LEG_CHANGE_INTERVAL 8000 //5000
#define SKIP_LEGS_DELAY 8000
#define POSITIONS 2

// How much time must stdev be more than ground level to detect leg pair is on ground.
#define IQR_MS 500 //2000
#define IQR_GROUND 0.0010 //0.0006

//original
static int toPositionLegPairs[POSITIONS][POSITIONS]  = {
    {LEG_A, LEG_B},
    {LEG_C, LEG_D}
};

//C je vodilna noga
//static int toPositionLegPairs[POSITIONS][POSITIONS]  = {
//    {LEG_C, LEG_D},
//    {LEG_A, LEG_B}
//};


int singleLegPos;
int legsOnGround;
unsigned long twoLegsTimer;
unsigned long legsDelayTimer;

void ToGroundController::setup(Leg *legs, GyroQuartiles *gyroQuartiles) {
    this->legs = legs;
    this->gyroQuartiles = gyroQuartiles;
    this->state = ToGroundState::None;
}

void ToGroundController::start() {
    state = ToGroundState::MovingTwoLegs;
    lastToPositionChange = 0;
    isLegPairWithinGroundLevels = false;
    toPositionCurrentPos = 0;
    singleLegPos = 0;
    legsOnGround = 0;
    twoLegsTimer = millis();
    legsDelayTimer = 0;
}

void ToGroundController::nextPosition() {
    lastToPositionChange = millis();
    LegUtil::stopAllMotors(legs);
    toPositionCurrentPos = (toPositionCurrentPos + 1) % POSITIONS;
}

ToGroundResult ToGroundController::loop() {
    if (state == ToGroundState::Done) {
        // All positions at end state
        return ToGroundResult::Done;
    } else if (state == ToGroundState::MovingTwoLegs) {
        moveTwoLegsLoop();
    } else if (state == ToGroundState::MovingSingleLeg) {
        moveSingleLegLoop();
    } else if (state == ToGroundState::Error) {
        return ToGroundResult::Error;
    } else {
        DPRINTLN(F("Unsupported final state!"));
        return ToGroundResult::Error;
    }

    return ToGroundResult::Moving;
}


bool shouldDelay() {
    // Don't do anything if time delayed
    return millis() - legsDelayTimer < SKIP_LEGS_DELAY;
}

// Don't start moving for some time. We want stdev to stabilize.
void delayLegs() {
    legsDelayTimer = millis();
}

// Ground is detected after standard deviation has been high enough for enough time
void ToGroundController::moveTwoLegsLoop() {
    if (shouldDelay()) {
        return;
    }

    for (int i = 0; i < POSITIONS; i++) {
        int legPos = toPositionLegPairs[toPositionCurrentPos][i];
        Leg *leg = &legs[legPos];
        leg->expand();
    }

    if (!gyroQuartiles->hasStats()) {
        DPRINTLN("No quartile stats available.");
        state = ToGroundState::Error;
    }

    if (gyroQuartiles->getIqrPitch() >= IQR_GROUND || gyroQuartiles->getIqrRoll() >= IQR_GROUND) {
        // Possible ground
        if (!isLegPairWithinGroundLevels) {
            groundLevelTimer = millis();
        }
        isLegPairWithinGroundLevels = true;
    }

    // If we've been on ground long enough detect on ground (mark position done).
    if (isLegPairWithinGroundLevels && ((millis() - groundLevelTimer) >= IQR_MS)) {
        DPRINTLN("One leg pair within ground level.");
        DPRINTLN("Start moving other legs ...");
        state = ToGroundState::MovingSingleLeg;
        isLegPairWithinGroundLevels = false;
        LegUtil::stopAllMotors(legs);
        delay(STOP_MOTORS_DLY);
        delayLegs();
        return;
    }

    if (millis() - twoLegsTimer > TO_POSITION_LEG_CHANGE_INTERVAL) {
        delay(STOP_MOTORS_DLY);
        LegUtil::stopAllMotors(legs);
        toPositionCurrentPos = (toPositionCurrentPos + 1) % POSITIONS;
        twoLegsTimer = millis();
    }
}

void ToGroundController::moveSingleLegLoop() {
    if (shouldDelay()) {
        return;
    }

    Leg *leg = &legs[singleLegPos];
    leg->expand();

    if (!gyroQuartiles->hasStats()) {
        DPRINTLN("No quartile stats available.");
        state = ToGroundState::Error;
    }

    if (gyroQuartiles->getIqrPitch() >= IQR_GROUND || gyroQuartiles->getIqrRoll() >= IQR_GROUND) {
        // Possible ground
        if (!isLegPairWithinGroundLevels) {
            groundLevelTimer = millis();
        }
        isLegPairWithinGroundLevels = true;
    }

    // If we've been on ground long enough detect on ground (mark position done).
    if (isLegPairWithinGroundLevels && ((millis() - groundLevelTimer) >= IQR_MS)) {
        DPRINT("Leg ");
        DPRINT(leg->getName());
        DPRINTLN(" within ground level.");
        leg->stop();
        isLegPairWithinGroundLevels = false;

        singleLegPos++;
        legsOnGround++;

        if (legsOnGround == MAX_LEGS) {
            state = ToGroundState::Done;
            return;
        }

        delay(STOP_MOTORS_DLY);
        delayLegs();
    }
}
