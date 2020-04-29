#include "ButtonView.h"

void ButtonView::setup(Balancer *balancer, GyroQuartiles *gyroQuartiles) {
    this->balancer = balancer;

    buttonUp.setup(buttonConfigUp);
    buttonCenter.setup(buttonConfigCenter);
    buttonDown.setup(buttonConfigDown);

    buttonUp.setOnPressCallback(&ButtonView::onUpPressCallback, this);
    buttonUp.setOnReleaseCallback(&ButtonView::onUpReleaseCallback, this);
    buttonCenter.setOnPressCallback(&ButtonView::onCenterPressCallback, this);
    buttonCenter.setOnReleaseCallback(&ButtonView::onCenterReleaseCallback, this);
    buttonDown.setOnPressCallback(&ButtonView::onDownPressCallback, this);
    buttonDown.setOnReleaseCallback(&ButtonView::onDownReleaseCallback, this);
    lastButton = &buttonUp;
}

void ButtonView::registerViewObserver(ViewObserver *viewObserver) {
    this->viewObserver = viewObserver;
}

void ButtonView::loop() {
    buttonUp.loop();
    buttonCenter.loop();
    buttonDown.loop();

    if (!hasRecentlySwitchedMode && buttonCenter.isPressed() && buttonCenter.getPressedButtonDurationMs() >= SERVICE_MODE_BUTTON_PRESS_MS) {
        setMode(mode == Mode::SERVICE ? Mode::NORMAL : Mode::SERVICE);
        if (mode == Mode::SERVICE)
            buttonCenter.setLedBlinking(500);
        else
            buttonCenter.setLedOff();
        buttonUp.setLedOff();
        buttonDown.setLedOff();
        hasRecentlySwitchedMode = true;
    }
}

bool isFinalBalancerState(State balancerState) {
    switch (balancerState) {
        case State::ZeroState:
        case State::FinalState:
        case State::BalancedState:
            return true;
            break;
        default:
            return false;
            break;
    }
}

void ButtonView::onUpPress() {
    CommandResult command = getCommandForUpDown(ViewCommand::TO_ZERO, MOTOR_REVERSE_STATE);
    onCommandCallbackSafe(command);
    buttonUp.setLedOn();
    buttonDown.setLedOff();
    lastButton = &buttonUp;
    if (mode != Mode::SERVICE)
        buttonCenter.setLedOff();
}

void ButtonView::onUpRelease() {
    // We want the led to keep blinking if final state was reached
    if (!isFinalBalancerState(balancer->getState()))
        buttonUp.setLedOff();

    CommandResult command = CommandResult(ViewCommand::STOP_ALL);
    onCommandCallbackSafe(command);
}

void ButtonView::onCenterPress() {
    hasRecentlySwitchedMode = false;
}

void ButtonView::onCenterRelease() {
    if (mode == Mode::NORMAL) {
        switch (centerButtonState) {
            case CenterButtonState::NONE:
                centerButtonState = CenterButtonState::BALANCE;
                onCommandCallbackSafe(CommandResult(ViewCommand::BALANCE));
                buttonCenter.setLedOn();
                break;
            case CenterButtonState::BALANCE:
                centerButtonState = CenterButtonState::STOP;
                onCommandCallbackSafe(CommandResult(ViewCommand::STOP_ALL));
                buttonCenter.setLedOff();
                break;
            case CenterButtonState::STOP:
                centerButtonState = CenterButtonState::TO_ZERO;
                onCommandCallbackSafe(CommandResult(ViewCommand::TO_ZERO));
                buttonCenter.setLedOn();
                break;
            case CenterButtonState::TO_ZERO:
                centerButtonState = CenterButtonState::NONE;
                onCommandCallbackSafe(CommandResult(ViewCommand::STOP_ALL));
                buttonCenter.setLedOff();
                break;
        }
    } else if (mode == Mode::SERVICE) {
        selectedLegId = (selectedLegId + 1) % MAX_LEGS;
    }
}

void ButtonView::onDownPress() {
    CommandResult command = getCommandForUpDown(ViewCommand::TO_FINAL, MOTOR_FORWARD_STATE);
    onCommandCallbackSafe(command);
    buttonUp.setLedOff();
    buttonDown.setLedOn();
    lastButton = &buttonDown;
    if (mode != Mode::SERVICE)
        buttonCenter.setLedOff();
}

void ButtonView::onDownRelease() {
    // We want the led to keep blinking if final state was reached
    if (!isFinalBalancerState(balancer->getState()))
        buttonDown.setLedOff();
    CommandResult command = CommandResult(ViewCommand::STOP_ALL);
    onCommandCallbackSafe(command);
}

void ButtonView::notifyStateChange(State newState) {
    switch (newState) {
        case State::ZeroState:
        case State::FinalState:
        case State::BalancedState:
            // Do not react if command run from center button
            if (lastButton->isPressed())
                lastButton->setLedBlinking(1000);
            if (mode == Mode::NORMAL)
                buttonCenter.setLedOff();
            break;
        case State::ErrorState:
            buttonCenter.setLedBlinking(2000);
            break;
        case State::NoState:
        case State::GroundState:
        case State::ToZeroState:
        case State::ToFinalState:
        case State::ToGroundState:
        case State::BalancingState:
            break;
    }
}

CommandResult ButtonView::getCommandForUpDown(ViewCommand normalViewCommand, int singleLegDirection) {
    switch (mode) {
        case Mode::NORMAL:
            return normalViewCommand;
        case Mode::SERVICE:
            {
                CommandResult commandResult(ViewCommand::START_SINGLE_MOTOR);
                commandResult.legId = selectedLegId;
                commandResult.direction = singleLegDirection;
                return commandResult;
            }
        default:
            return CommandResult(ViewCommand::NONE);
    }
}

void ButtonView::setMode(Mode mode) {
    this->mode = mode;
}

void ButtonView::onCommandCallbackSafe(CommandResult commandResult) {
    if (viewObserver != NULL) {
        viewObserver->onCommand(commandResult);
    }
}

// http://tedfelix.com/software/c++-callbacks.html
void ButtonView::onUpPressCallback(void *p) {
    ((ButtonView *)p) -> onUpPress();
}

void ButtonView::onUpReleaseCallback(void *p) {
    ((ButtonView *)p) -> onUpRelease();
}

void ButtonView::onCenterPressCallback(void *p) {
    ((ButtonView *)p) -> onCenterPress();
}

void ButtonView::onCenterReleaseCallback(void *p) {
    ((ButtonView *)p) -> onCenterRelease();
}

void ButtonView::onDownPressCallback(void *p) {
    ((ButtonView *)p) -> onDownPress();
}

void ButtonView::onDownReleaseCallback(void *p) {
    ((ButtonView *)p) -> onDownRelease();
}

