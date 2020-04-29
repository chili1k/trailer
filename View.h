#ifndef View_h
#define View_h

#include "TrailerStateObserver.h"
#include "Balancer.h"
#include "GyroQuartiles.h"
#include "ViewObserver.h"

class View: public TrailerStateObserver {
    public:
        virtual void setup(Balancer *balancer, GyroQuartiles *gyroQuartiles) = 0;
        // View to controller event
        virtual void registerViewObserver(ViewObserver *viewObserver) = 0;
        // Controller to view event
        virtual void notifyStateChange(State newState) = 0;
        virtual void loop() = 0;
};

#endif
