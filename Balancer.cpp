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
  balancingState = BalancingState::NotBalancing;
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
  /*
  if (getState() != State::GroundState) {
    DPRINTLN(F("Trailer needs to be on ground!"));
    setState(State::NoState);
    return;
  }
  */

  // Trailer can only be balanced if all legs are on ground
  if (gyro.isBalanced()) { // && LegUtil::allLegsOnGround(legs)) {
    DPRINTLN(F("Trailed BALANCED!"));
    LegUtil::stopAllMotors(legs);
    setState(State::BalancedState);
    return;
  }

  // If all legs in final position
  if (LegUtil::anyLegInPosition(legs, LegPosition::Final)) {
    DPRINTLN(F("Cannot balance. One of the legs reached final position."));
    LegUtil::stopAllMotors(legs);
    setState(State::ErrorState);
    return;
  }

  if (balancingState == BalancingState::NotBalancing) {
    determineBalancingState();
    if (balancingState == BalancingState::NotBalancing) {
      DPRINTLN(F("Cannot balance! Unknown gyro position."));
      setState(State::ErrorState);
      return;
    }

    expandLegs(balancingAction.step1Legs[0], balancingAction.step1Legs[1]);
  }

  switch (balancingState) {
    case BalancingState::Step1:
      loopBalancingStep1();
      break;
    case BalancingState::Step2:
      loopBalancingStep2();
      break;
    case BalancingState::Done:
      if (!gyro.isBalanced()) {
        DPRINTLN(F("Cannot Balancing. All balancing steps done."));
        setState(State::ErrorState);
      }
      break;
    default:
      DPRINTLN(F("Invalid balancing state!"));
      setState(State::ErrorState);
  }
}

void Balancer::loopBalancingStep1() {
  bool stepDone = (balancingAction.step1Axe == Axe::Pitch && gyro.isPitchBalanced())
                || (balancingAction.step1Axe == Axe::Roll && gyro.isRollBalanced());

  if (stepDone) {
    if (balancingAction.steps == 1) {
      balancingState = BalancingState::Done;
    } else if (balancingAction.steps == 2) {
      balancingState = BalancingState::Step2;
      expandLegs(balancingAction.step2Legs[0], balancingAction.step2Legs[1]);
    } else {
      // should really not happen
      DPRINTLN(F("Unknown number of steps."));
      setState(State::ErrorState);
      return;
    }
  }
}

void Balancer::loopBalancingStep2() {
  bool stepDone = (balancingAction.step2Axe == Axe::Pitch && gyro.isPitchBalanced())
                || (balancingAction.step2Axe == Axe::Roll && gyro.isRollBalanced());

  if (stepDone) {
    balancingState = BalancingState::Done;
  }
}

void Balancer::determineBalancingState() {
  float pitch = gyro.getPitch();
  float roll = gyro.getRoll();

  if (pitch == 0.0 && roll > 0.0) {
    balancingAction.steps = 1;
    balancingAction.step1Axe = Axe::Roll;
    balancingAction.step1Legs[0] = &legs[LEG_C];
    balancingAction.step1Legs[1] = &legs[LEG_D];
  } else if (pitch == 0.0 && roll < 0.0) {
    balancingAction.steps = 1;
    balancingAction.step1Axe = Axe::Roll;
    balancingAction.step1Legs[0] = &legs[LEG_A];
    balancingAction.step1Legs[1] = &legs[LEG_B];
  } else if (pitch > 0.0 && roll == 0.0) {
    balancingAction.steps = 1;
    balancingAction.step1Axe = Axe::Pitch;
    balancingAction.step1Legs[0] = &legs[LEG_D];
    balancingAction.step1Legs[1] = &legs[LEG_B];
  } else if (pitch > 0.0 && roll > 0.0) {
    balancingAction.steps = 2;
    balancingAction.step1Axe = Axe::Pitch;
    balancingAction.step1Legs[0] = &legs[LEG_D];
    balancingAction.step1Legs[1] = &legs[LEG_B];
    balancingAction.step2Axe = Axe::Roll;
    balancingAction.step2Legs[0] = &legs[LEG_C];
    balancingAction.step2Legs[1] = &legs[LEG_D];
  } else if (pitch > 0.0 && roll < 0.0) {
    balancingAction.steps = 2;
    balancingAction.step1Axe = Axe::Pitch;
    balancingAction.step1Legs[0] = &legs[LEG_D];
    balancingAction.step1Legs[1] = &legs[LEG_B];
    balancingAction.step2Axe = Axe::Roll;
    balancingAction.step2Legs[0] = &legs[LEG_A];
    balancingAction.step2Legs[1] = &legs[LEG_B];
  } else if (pitch < 0.0 && roll == 0.0) {
    balancingAction.steps = 1;
    balancingAction.step1Axe = Axe::Pitch;
    balancingAction.step1Legs[0] = &legs[LEG_A];
    balancingAction.step1Legs[1] = &legs[LEG_C];
  } else if (pitch < 0.0 && roll > 0.0) {
    balancingAction.steps = 2;
    balancingAction.step1Axe = Axe::Pitch;
    balancingAction.step1Legs[0] = &legs[LEG_A];
    balancingAction.step1Legs[1] = &legs[LEG_C];
    balancingAction.step2Axe = Axe::Roll;
    balancingAction.step2Legs[0] = &legs[LEG_C];
    balancingAction.step2Legs[1] = &legs[LEG_D];
  } else if (pitch < 0.0 && roll < 0.0) {
    balancingAction.steps = 2;
    balancingAction.step1Axe = Axe::Pitch;
    balancingAction.step1Legs[0] = &legs[LEG_A];
    balancingAction.step1Legs[1] = &legs[LEG_C];
    balancingAction.step2Axe = Axe::Roll;
    balancingAction.step2Legs[0] = &legs[LEG_A];
    balancingAction.step2Legs[1] = &legs[LEG_B];
  } else {
    return;
  }

  if (balancingAction.steps == 2) {
    // reverse steps if roll > pitch
    if (abs(roll) > abs(pitch)) {
      Leg *leg1 = balancingAction.step1Legs[0];
      Leg *leg2 = balancingAction.step1Legs[1];

      balancingAction.step1Axe = Axe::Roll;
      balancingAction.step1Legs[0] = balancingAction.step2Legs[0];
      balancingAction.step1Legs[1] = balancingAction.step2Legs[1];
      balancingAction.step2Axe = Axe::Pitch;
      balancingAction.step2Legs[0] = leg1;
      balancingAction.step2Legs[1] = leg2;
    }
  }

  balancingState = BalancingState::Step1;
}

void Balancer::expandLegs(Leg *leg1, Leg *leg2) {
  LegUtil::stopAllMotors(legs);
  // we don't want to be restarting too fast
  delay(STOP_MOTORS_DLY);
  leg1->expand();
  leg2->expand();
  // expand legs at least one second before trying to expand another leg
  delay(EXPAND_LEG_DLY);
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
