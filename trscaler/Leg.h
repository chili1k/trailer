/*
  Leg.h - Leg position
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Leg_h
#define Leg_h

enum LegPosition { Collapsed, Expanding, Levelling, Final, Error };

class Leg {
  public:
    Leg(int pinCollapsedPos, int pinFinalPos, int pinPowerMeter);
    LegPosition getPosition();
  private:
    int pinCollapsedPos;
    int pinFinalPos;
    int pinPowerMeter;
    bool isHighAmperage;
    void setup();
    bool isHighAmperage();
};

#endif
