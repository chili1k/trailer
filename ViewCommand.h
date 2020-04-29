#ifndef ViewCommand_h
#define ViewCommand_h

enum ViewCommand { STOP_ALL, BALANCE, TO_ZERO, TO_GROUND, TO_FINAL, START_SINGLE_MOTOR, SET_PWM, NONE };

struct CommandResult {
    CommandResult() {
        this->command = ViewCommand::NONE;
    }

    CommandResult(ViewCommand viewCommand) {
        this->command = viewCommand;
    }

    ViewCommand command;
    bool isError;
    int legId;
    int direction;
    int pwm;

};

#endif
