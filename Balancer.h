#ifndef Balancer_h 
#define Balancer_h 

#include "Display.h"
#include "Leg.h"
#include "Gyro.h"
#include "Config.h"

enum State { NoState, ZeroState, ToZeroState, BalancedState, BalancingState, FinalState, ErrorState };

class Balancer {
  public:
    Balancer(Leg **legs, Gyro *gyro, Display *display);
    void setup();
    void loop();

  private:
    Leg **legs;
    Gyro *gyro;
    Display *display;
    State state;

    void setState(State newState);
    void toZero();
    void balance();

    void expandLeg(Leg *leg);

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
