#ifndef ViewObserver_h
#define ViewObserver_h

#include "ViewCommand.h"

class ViewObserver {
    public:
        virtual void onCommand(const CommandResult commandResult) = 0;
};

#endif
