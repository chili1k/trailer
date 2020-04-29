#ifndef ButtonView_h
#define ButtonView_h

#include "LedButton.h"
#include "BalancerState.h"
#include "View.h"
#include "SerialDebugWriter.h"
#include "Leg.h"
#include "util/Timer.h"

enum class CenterButtonState { NONE, BALANCE, STOP, TO_ZERO };
enum class Mode { NORMAL, SERVICE };

class ButtonView : public View {
    public:
        void setup(Balancer *balancer, GyroQuartiles *gyroQuartiles);
        void registerViewObserver(ViewObserver *viewObserver);
        void loop();
        void notifyStateChange(State newState);

        CommandResult readInput();

        // Yuck :( but there is no other way
        static void onUpPressCallback(void *p);
        static void onUpReleaseCallback(void *p);
        static void onCenterPressCallback(void *p);
        static void onCenterReleaseCallback(void *p);
        static void onDownPressCallback(void *p);
        static void onDownReleaseCallback(void *p);

    private:
        void setMode(Mode mode);
        void onUpPress();
        void onUpRelease();
        void onCenterPress();
        void onCenterRelease();
        void onDownPress();
        void onDownRelease();

        CommandResult getCommandForUpDown(ViewCommand normalViewCommand, int singleLegDirection);

        Balancer *balancer;
        LedButton buttonUp;
        LedButton buttonCenter;
        LedButton buttonDown;
        LedButton *lastButton;

        // State
        Mode mode = Mode::NORMAL;
        CenterButtonState centerButtonState = CenterButtonState::NONE;
        ViewObserver *viewObserver = NULL;
        void onCommandCallbackSafe(CommandResult commandResult);
        int selectedLegId = 0;
        bool hasRecentlySwitchedMode = false;
};

#endif
