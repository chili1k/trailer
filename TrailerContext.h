#ifndef TrailerContext_h
#define TrailerContext_h

class TrailerState;

class TrailerContext {
  public:
    TrailerContext(Gyro *gyro, Leg **legs);
    void setState(TrailerState *state);
    void setup();
    void loop();
    void stopAllMotors();
    bool allLegsInPosition(LegPosition position);
    Gyro *getGyro();
    Leg **getLegs();

    TrailerState *getZeroState();
    TrailerState *getToZeroState();
    TrailerState *getExpandingToGroundState();
    TrailerState *getBalancedState();
    TrailerState *getBalancingState();
    TrailerState *getErrorState();
  private:
    TrailerState *state;
    // LegA, LegB, LegC, LegD
    Gyro *gyro;
    Leg **legs;

    TrailerState zeroState;
};

#endif
