#ifndef Balancer_h 
#define Balancer_h 

#include "Leg.h"
#include "Gyro.h"
#include "Config.h"

enum State { NoState, ZeroState, ToZeroState, BalancedState, BalancingState, FinalState, ErrorState };

class Balancer {
  public:
    Balancer(Leg **legs, Gyro *gyro);
    void setup();
    void loop();
    /**
    * Move trailer to zero position
    */
    void toZero();
    /**
    * Balance trailer
    */
    void balance();
    Leg **getLegs();
    Gyro *getGyro();
    State *getState();

  private:
    Leg **legs;
    Gyro *gyro;
    State state;

    void setState(State newState);

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
