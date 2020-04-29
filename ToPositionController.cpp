/**
 * Moves legs to zero or final.
 */

#include "Config.h"
#include "ToPositionController.h"
#include "LegUtil.h"
#include "Leg.h"
#include "Debug.h"

#define TO_POSITION_POSITIONS 2
#define TO_POSITION_LEG_COUNT 2
#define TO_POSITION_LEG_CHANGE_INTERVAL 5000

int toPositionLegPairs[TO_POSITION_POSITIONS][TO_POSITION_LEG_COUNT]  = {
    {LEG_A, LEG_B},
    {LEG_C, LEG_D}
};

void ToPositionController::setup(Leg *legs) {
    this->legs = legs;
}

void ToPositionController::reset(State desiredState) {
    this->desiredState = desiredState;
    this->lastToPositionChange = 0;
    this->toPositionCurrentPos = 0;
    this->positionsOnEndState = 0;

    if (desiredState == State::ToZeroState) {
        desiredLegState = LegPosition::Zero;
    } else if (desiredState == State::ToFinalState) {
        desiredLegState = LegPosition::Final;
    }
}

void ToPositionController::nextPosition() {
    lastToPositionChange = millis();
    LegUtil::stopAllMotors(legs);
    toPositionCurrentPos = (toPositionCurrentPos + 1) % TO_POSITION_POSITIONS;
}

ToPositionResult ToPositionController::loop() {
    // All positions at end state
    if (positionsOnEndState == TO_POSITION_POSITIONS) {
        return ToPositionResult::Done;
    }

    if (desiredState == State::ToZeroState || desiredState == State::ToFinalState) {
        moveToFinalOrZeroLoop();
    } else {
        DPRINTLN(F("Unsupported final state!"));
        return ToPositionResult::Error;
    }

    // Switch leg positions in pairs and on interval. Do not switch if one position is already in final state.
    if (positionsOnEndState == 0 && millis() - lastToPositionChange > TO_POSITION_LEG_CHANGE_INTERVAL) {
        delay(STOP_MOTORS_DLY);
        nextPosition();
    }

    return ToPositionResult::Moving;
}

void ToPositionController::moveToFinalOrZeroLoop() {
    int c = 0;
    for (int i = 0; i < TO_POSITION_LEG_COUNT; i++) {
        int legPos = toPositionLegPairs[toPositionCurrentPos][i];
        Leg *leg = &legs[legPos];

        if (leg->getPosition() == desiredLegState) {
            c++;
            leg->stop();
        } else {
            if (desiredLegState == LegPosition::Zero)
                leg->collapse();
            else
                leg->expand();
        }
    }

    if (c == TO_POSITION_LEG_COUNT) {
        positionsOnEndState++;
        nextPosition();
    }
}
