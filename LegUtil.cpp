#include "Leg.h"
#include "LegUtil.h"

void LegUtil::stopAllMotors(Leg **legs) {
  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i]->stop();
  }
}

bool LegUtil::allLegsOnGround(Leg **legs) {
  for (int i = 0; i < MAX_LEGS; i++) {
    if (!legs[i]->isOnGround()) {
      return false;
    }
  }

  return true;
}

bool LegUtil::allLegsInPosition(Leg **legs, LegPosition position) {
  for (int i = 0; i < MAX_LEGS; i++) {
    if (legs[i]->getPosition() != position) {
      return false;
    }
  }

  return true;
}

