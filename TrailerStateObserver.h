#ifndef TrailerStateObserver_h
#define TrailerStateObserver_h

#include "BalancerState.h"

class TrailerStateObserver {
    public:
        virtual void notifyStateChange(State newState) = 0;
};

#endif
