#ifndef Balancer_h 
#define Balancer_h 

#include "Leg.h"
#include "Gyro.h"
#include "Config.h"

enum class State { NoState, ZeroState, ToZeroState, ToGroundState, ToFinalState, BalancedState, BalancingState, GroundState, FinalState, ErrorState };
enum class BalancingState { NotBalancing, Balancing, Done, Error };

struct BalancingAction {
  int tries;
  int badDeltas;
  Axe axe;
  Leg *legs[2];
  float previousPosition;
};


/*
Simple mode:
1 LED:
If blinking, then error state

4 Keys:
ON/OFF - Power switch key
Emergency stop key (RESET)
Auto level
Legs all up (Zero state)

Advanced mode:
5 Leds:
1 error
LEG 1
LEG 2
LEG 3
LEG 4

5 keys:
ON/OFF - Power switch key
Emergency stop key (RESET)
Auto level/Select leg
Legs up/Leg up
Legs down/Leg down

Auto level
Zero state
Choose leg
Leg Up
Leg Down


*/

class Balancer {
  public:
    void setup();
    void loop();
    // START Commands
    void toZero();
    void toGround();
    void toFinal();
    void balance();
    void stopAllLegs();
    void forceExpandLeg(int legId);
    void forceCollapseLeg(int legId);
    // END Commands

    Leg *getLegs();
    Gyro *getGyro();
    State getState();

  private:
    Leg legs[MAX_LEGS] = { Leg(legConfigA), Leg(legConfigB), Leg(legConfigC), Leg(legConfigD) };
    Gyro gyro;
    State state = State::NoState;
    BalancingState balancingState = BalancingState::NotBalancing;
    BalancingAction balancingAction;

    void setState(State newState);

    void expandLegs(Leg *leg1, Leg *leg2);
    void determineBalancingState();

    void loopBalancingStep();
    bool isBadDelta(float newPosition);

    void stateToZeroLoop();
    void stateToGroundLoop();
    void stateToFinalLoop();
    void stateBalancingLoop();
};

#endif
