#ifndef Balancer_h 
#define Balancer_h 

#include "Leg.h"
#include "Gyro.h"
#include "Config.h"

enum class State { NoState, ZeroState, ToZeroState, BalancedState, BalancingState, FinalState, ErrorState };


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
    /**
    * Move trailer to zero position
    */
    void toZero();
    /**
    * Balance trailer
    */
    void balance();
    void stopAllLegs();
    // END Commands

    Leg *getLegs();
    Gyro *getGyro();
    State getState();

  private:
    Leg legs[MAX_LEGS] = { Leg(legConfigA), Leg(legConfigB), Leg(legConfigC), Leg(legConfigD) };
    Gyro gyro;
    State state = State::NoState;

    void setState(State newState);

    void expandLeg(Leg &leg);

    void stateNoStateLoop();
    void stateNoStateCmdToZero();
    void stateNoStateCmdBalance();

    void stateZeroLoop();
    void stateZeroCmdToZero();
    void stateZeroCmdBalance();

    void stateToZeroLoop();
    void stateToZeroCmdToZero();
    void stateToZeroCmdBalance();

    void stateBalancingLoop();
    void stateBalancingCmdToZero();
    void stateBalancingCmdBalance();

    void stateBalancedLoop();
    void stateBalancedCmdToZero();
    void stateBalancedCmdBalance();

    void stateFinalLoop();
    void stateFinalCmdToZero();
    void stateFinalCmdBalance();

    void stateErrorLoop();
    void stateErrorCmdToZero();
    void stateErrorCmdBalance();
};

#endif
