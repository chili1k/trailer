/*
  Leg.h - Leg position
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Leg_h
#define Leg_h

enum LegPosition { Zero, Expanding, Ground, Final };

class Leg {
  public:
    Leg(int pinZeroPos, int pinFinalPos, int pinPowerMeter);
    LegPosition getPosition();
    
  private:
    int pinZeroPos;
    int pinFinalPos;
    int pinPowerMeter;
    void setup();
    bool isHighAmperage();
};

#endif
