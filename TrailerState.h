#ifndef TrailerState_h
#define TrailerState_h

#include "Display.h"
#include "Leg.h"
#include "Gyro.h"
#include "TrailerContext.h"

class TrailerState {
  protected:
    Display display;
    TrailerContext *context;
    Leg **legs;
    Gyro **gyro;
  public:
    TrailerState(TrailerContext *context) {
      this->context = context;
      this->legs = context->getLegs();
      this->gyro = context->getGyro();
    }
    virtual void loop() = 0;
    virtual void toZero() = 0;
    virtual void balance() = 0;
};

class ZeroState : public TrailerState {
  public:
    ZeroState(TrailerContext *context) : TrailerState(TrailerContext *context);
    void loop();
    void toZero();
    void balance();
};

#endif
