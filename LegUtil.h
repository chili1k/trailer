#ifndef LegUtil_h
#define LegUtil_h

#include "Leg.h"

class LegUtil {
  public:
    static void stopAllMotors(Leg **leg);
    static bool allLegsOnGround(Leg **legs);
    static bool allLegsInPosition(Leg **legs, LegPosition position);
};

#endif
