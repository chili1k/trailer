#include "Balancer.h"
#include "Gyro.h"
#include "Leg.h"
#include "LegUtil.h"
#include "Debug.h"
#include "Config.h"

#include <Arduino.h>

void Balancer::setup() {
  gyro.setup();

  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i].setup();
  }
}

void Balancer::loop() {
  gyro.loop();

  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i].loop();
  }

  switch (state) {
    case State::NoState:
    case State::ZeroState:
    case State::BalancedState:
    case State::ErrorState:
    case State::GroundState:
    case State::FinalState:
      break;
    case State::BalancingState:
      stateBalancingLoop();
      break;
    case State::ToZeroState:
      stateToZeroLoop();
      break;
    case State::ToGroundState:
      stateToGroundLoop();
      break;
    case State::ToFinalState:
      stateToFinalLoop();
      break;
    default:
      DPRINTLN(F("Unknown state."));
  }
}

void Balancer::toZero() {
  setState(State::ToZeroState);
}

void Balancer::toGround() {
  setState(State::ToGroundState);
}

void Balancer::toFinal() {
  setState(State::ToFinalState);
}

void Balancer::balance() {
  setState(State::BalancingState);
}

void Balancer::forceExpandLeg(int legId) {
  // Same for all states.
  stopAllLegs();
  legs[legId].expand();
  setState(State::NoState);
}

void Balancer::forceCollapseLeg(int legId) {
  // Same for all states.
  stopAllLegs();
  legs[legId].collapse();
  setState(State::NoState);
}

void Balancer::stopAllLegs() {
  LegUtil::stopAllMotors(legs);
  setState(State::NoState);
}

void Balancer::setState(State newState) {
  state = newState;
}

void Balancer::stateToZeroLoop() {
  int c = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    Leg leg = legs[i];

    if (leg.getPosition() == LegPosition::Zero) {
      c++;
      leg.stop();
//    } else if (leg.getPosition() != LegPosition::Unknown && leg.getPosition() != LegPosition::Zero) {
    } else if (leg.getPosition() != LegPosition::Zero) {
      leg.collapse();
    }
  }

  if (c == MAX_LEGS) {
    setState(State::ZeroState);
  }
}

void Balancer::stateToGroundLoop() {
  int c = 0;
  int isFinalPosition = false;

  for (int i = 0; i < MAX_LEGS; i++) {
    Leg leg = legs[i];

    if (leg.isOnGround()) {
      c++;
      leg.stop();
    } else if (leg.getPosition() == LegPosition::Final) {
      DPRINTLN(F("STOP To Ground operation. Leg at final position detected."));
      isFinalPosition = true;
      break;
    } else {
      leg.expand();
    }
  }

  if (c == MAX_LEGS) {
    setState(State::GroundState);
  }

  if (isFinalPosition) {
    stopAllLegs();
    setState(State::ErrorState);
  }
}

void Balancer::stateToFinalLoop() {
  int c = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    Leg leg = legs[i];

    if (leg.getPosition() == LegPosition::Final) {
      c++;
      leg.stop();
    } else if (leg.getPosition() != LegPosition::Final) {
      leg.expand();
    }
  }

  if (c == MAX_LEGS) {
    setState(State::ZeroState);
  }
}

void Balancer::stateBalancingLoop() {
  if (getState() != State::GroundState) {
    DPRINTLN(F("Trailer needs to be on ground!"));
    return;
  }

  // Trailer can only be balanced if all legs are on ground
  if (gyro.isBalanced() && LegUtil::allLegsOnGround(legs)) {
    DPRINTLN(F("Trailed BALANCED!"));
    LegUtil::stopAllMotors(legs);
    setState(State::BalancedState);
    return;
  }

  // If all legs in final position
  if (LegUtil::allLegsInPosition(legs, LegPosition::Final)) {
    DPRINTLN(F("Cannot balance. All legs reached final position."));
    LegUtil::stopAllMotors(legs);
    setState(State::ErrorState);
    return;
  }

  // Could also be A and C at the same time
  if (gyro.getPitchPosition() == UnderBalanced && gyro.getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_A]);
  } else if (gyro.getPitchPosition() == UnderBalanced && gyro.getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_B]);
  // Could also be B and D at the same time
  } else if (gyro.getPitchPosition() == OverBalanced && gyro.getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_C]);
  } else if (gyro.getPitchPosition() == OverBalanced && gyro.getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_D]);
  }
}

void Balancer::expandLeg(Leg &leg) {
  // Start only if not already started. Stop any other motors.
  if (leg.isMotorStopped()) {
    LegUtil::stopAllMotors(legs);
    // We don't want to be restarting too fast
    delay(STOP_MOTORS_DLY);
    leg.expand();
    // Expand leg at least one second before trying to expand another leg
    delay(EXPAND_LEG_DLY);
  }
}

Leg *Balancer::getLegs() {
  return legs;
}

Gyro *Balancer::getGyro() {
  return &gyro;
}

State Balancer::getState() {
  return state;
}
