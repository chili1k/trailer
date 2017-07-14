#include "TrailerState.h"
#include "TrailerContext.h"

/////// Zero state

void ZeroState::loop() {
  // Nothing to do
}

void ZeroState::toZero() {
  display.println("Already at zero.");
}

void ZeroState::balance() {
  context->setState(context->getBalancingState());
}

/////// ToZero state

class ToZeroState : public TrailerState {
  public:
    void loop();
    void toZero();
    void balance();
};

ToZeroState::ToZeroState(TrailerContext *context) {
  this->context = context;
  this->legs = context->getLegs();
}

void ToZeroState::loop() {
  int zeroCount = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    Leg *leg = legs[i];

    if (leg->getPosition() == Zero) {
      leg->stop();
      zeroCount++;
    } else if (leg->getPosition() != Unknown && leg->getPosition() != Zero) {
      leg->collapse();
    }
  }

  if (zeroCount == MAX_LEGS) {
    context->setState(context->getZeroState());
  }
}

void ToZeroState::toZero() {
  display.println("Already at zero.");
}

void ToZeroState::balance() {
  context->setState(context->getBalancingState());
}
/////// Balancing state

class BalancingState : public TrailerState {
  private:
    void expandLeg(Leg *leg); 
    bool allLegsOnGround();
  public:
    void loop();
    void toZero();
    void balance();
};

void BalancingState::loop() {
  // Trailer can only be balanced if all legs are on ground
  if (gyro->isBalanced() && allLegsOnGround()) {
    display.println("Trailed BALANCED!");
    context->stopAllMotors();
    context->setState(context->getBalancedState());
    return;
  }

  // If all legs in final position
  if (context->allLegsInPosition(Final)) {
    display.println("Cannot balance. All legs reached final position.");
    context->stopAllMotors();
    context->setState(context->getErrorState());
  }

  // Could also be A and C at the same time
  if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_A]);
  } else if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_B]);
  // Could also be B and D at the same time
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_C]);
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_D]);
  }
}

bool BalancingState::allLegsOnGround() {
  int c = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    if (legs[i]->isOnGround()) {
      c++;
    }
  }

  return c == MAX_LEGS;
}

void BalancingState::expandLeg(Leg *leg) {
  // Start only if not already started. Stop any other motors.
  if (leg->isMotorStopped()) {
    context->stopAllMotors();
    leg->expand();
  }
}

void BalancingState::toZero() {
  context->stopAllMotors();
  context->setState(context->getToZeroState());
}

void BalancingState::balance() {
  // Already balancing
  display.println("Already balancing.");
}

/////// Balanced state

class BalancedState : public TrailerState {
  public:
    void loop();
    void toZero();
    void balance();
};

void BalancedState::loop() {
  // Nothing to do
}

void BalancedState::toZero() {
  context->setState(context->getToZeroState());
}

void BalancedState::balance() {
  // Nothing to do
}

/////// Final state

class FinalState : public TrailerState {
  public:
    void loop();
    void toZero();
    void balance();
};

void FinalState::loop() {
  // Nothing to do
}

void FinalState::toZero() {
  context->setState(context->getToZeroState());
}

void FinalState::balance() {
  display.println("Move to zero position first.");
}

/////// Error state

class ErrorState : public TrailerState {
  public:
    void loop();
    void toZero();
    void balance();
};

void ErrorState::loop() {
  // Nothing to do
}

void ErrorState::toZero() {
  context->setState(context->getToZeroState());
}

void ErrorState::balance() {
  // Nothing to do
  display.println("Move to zero position first.");
}

