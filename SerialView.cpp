#include "SerialView.h"
#include "Config.h"
#include "Ver.h"

void SerialView::setup(Balancer *balancer, GyroQuartiles *gyroQuartiles) {
    Serial.begin(SERIAL_BAUD_RATE);

    this->balancer = balancer;
    this->legs = balancer->getLegs();
    this->gyro = balancer->getGyro();
    this->gyroQuartiles = gyroQuartiles;

    #ifdef SERIAL_DEBUG_ENABLE
    serialDebugWriter.setup();
    serialDebugTimer.start();
    #endif

    printTitle();
}

void SerialView::loop() {
    refreshDisplay();
    readInput();

    #ifdef SERIAL_DEBUG_ENABLE
    refreshSerialDebug();
    #endif
}

void SerialView::notifyStateChange(State newState) {
    // No need. Polling does the job.
}

void SerialView::registerViewObserver(ViewObserver *viewObserver) {
    this->viewObserver = viewObserver;
}

void SerialView::readInput() {
    CommandResult commandResult(ViewCommand::NONE);

    if (Serial.available() > 0) {
        int b = Serial.read();

        switch (b) {
            case '0':
                commandResult.command = ViewCommand::STOP_ALL;
                break;
            case '1':
                commandResult.command = ViewCommand::BALANCE;
                break;
            case '2':
                commandResult.command = ViewCommand::TO_ZERO;
                break;
            case '3':
                commandResult.command = ViewCommand::TO_GROUND;
                break;
            case '4':
                commandResult.command = ViewCommand::TO_FINAL;
                break;
            case '5':
                commandResult.command = ViewCommand::START_SINGLE_MOTOR;
                readMotorInput(&commandResult);
                break;
            case '6':
                commandResult.command = ViewCommand::SET_PWM;
                readPWM(commandResult);
                break;
            default:
                DPRINTLN(F("Unknown command"));
        }

        if (viewObserver != NULL) {
            viewObserver->onCommand(commandResult);
        }
    }
}

int readNumber() {
    char buff[10];
    int nRead = 0;

    while (nRead < 5) {
        while (!Serial.available()) { }

        int c = Serial.read();
        if (c >= '0' && c <= '9') {
            Serial.print((char )c);
            buff[nRead++] = c;
        }
        if (c == '\r' || c == '\n') {
            Serial.println();
            break;
        }
    }

    buff[nRead] = '\0';

    return atoi(buff);
}

void SerialView::readMotorInput(CommandResult *commandResult) {
    int motorId = readMotor();
    if (motorId == -1) {
        commandResult->isError = true;
        return;
    }

    commandResult->legId = motorId;

    Serial.print(F("Choose direction [0-Expand 1-Collapse]: "));
    while (!Serial.available()) { }
    int direction = Serial.read()-'0';
    Serial.println(direction);
    if (direction != 0 && direction != 1) {
        commandResult->isError = true;
        Serial.println(F("WARNING: direction must be 0 or 1."));
        return;
    }
    commandResult->direction = direction;

    Serial.print(F("Running motor '"));
    Serial.print(motorId);

    if (direction == 0) {
        Serial.println(F("' in Expand direction..."));
    } else {
        Serial.println(F("' in Collapse direction..."));
    }
}

void SerialView::readPWM(CommandResult &commandResult) {
    int motorId = readMotor();
    if (motorId == -1) {
        commandResult.isError = true;
        return;
    }

    commandResult.legId = motorId;

    Serial.print("Enter value 0-255: ");
    int pwm = readNumber();

    if (pwm < 0 || pwm > 255) {
        commandResult.isError = true;
        Serial.println("Invalid PWM.");
        return;
    }

    commandResult.pwm = pwm;

    Serial.print("Setting motor ");
    Serial.print(motorId);
    Serial.print(" to PWM ");
    Serial.println(pwm);
}

int SerialView::readMotor() {
    // Same for all states.
    Serial.print(F("Choose motor [A-D]: "));
    while (!Serial.available()) { }
    int motorId = Serial.read()-'a';
    Serial.println(motorId);
    if (motorId < 0 || motorId >= MAX_LEGS) {
        Serial.println(F("WARNING: motor must be between 0 and 3."));
        return -1;
    }
    return motorId;
}

void SerialView::refreshDisplay() {
    unsigned long now = millis();

    // Do not refresh display often
    if ((now - lastRefreshTime) < DISPLAY_REFRESH_MS) {
        return;
    }

    timestamp++;
    lastRefreshTime = now;

    printHeader();

    for (int i = 0; i < MAX_LEGS; i++) {
        printLeg(&legs[i]);
    }

    Serial.println();
    printGyro();
    Serial.println();
    printTrailerState();
    Serial.println(F("--------------------------------------"));
    printCommands();
    Serial.println();
}

void SerialView::printCommands() {
    Serial.println(F("0: Stop all motors"));
    Serial.println(F("1: Balance trailer"));
    Serial.println(F("2: Return to zero"));
    Serial.println(F("3: Return to ground"));
    Serial.println(F("4: Move to final"));
    Serial.println(F("5: Start single motor"));
    Serial.println(F("6: Set PWM"));
}

void SerialView::printHeader() {
    Serial.print(F("#"));
    Serial.println(timestamp);
    Serial.println(F("Motor\tState\t\tPWM\tCurrent"));
}

void SerialView::printTrailerState() {
    State state = balancer->getState();

    Serial.print(F("Trailer state: "));

    switch (state) {
        case State::NoState:
            Serial.println(F("No State"));
            break;
        case State::ZeroState:
            Serial.println(F("At Zero Position"));
            break;
        case State::ToZeroState:
            Serial.println(F("Moving to Zero Position ..."));
            break;
        case State::ToGroundState:
            Serial.println(F("Moving to Ground Position ..."));
            break;
        case State::BalancedState:
            Serial.println(F("Balanced"));
            break;
        case State::BalancingState:
            Serial.println(F("Balancing ..."));
            break;
        case State::GroundState:
            Serial.println(F("At ground position"));
            break;
        case State::FinalState:
            Serial.println(F("At final position"));
            break;
        case State::ErrorState:
            Serial.println(F("Position error."));
            break;
        default:
            Serial.println(F("Unknown trailer state!"));
    }
}

void SerialView::printLeg(Leg *leg) {
    Serial.print(leg->getName());
    Serial.print(F("\t"));
    printLegPosition(leg);
    Serial.print(F("\t\t"));
    Serial.print(leg->getPWM());
    Serial.print(F("\t"));
    Serial.println();
}

void SerialView::printGyro() {
    float *ypr = gyro->getYPR();
    float pitch = ypr[1];
    float roll = ypr[2];

    Serial.print(F("Gyro pitch, roll: [ "));
    Serial.print(pitch, 4);
    Serial.print(F(", "));
    Serial.print(roll, 4);
    Serial.print(F(" ]"));

    if (gyroQuartiles->hasStats()) {
        Serial.println();
        Serial.print(F("iqr: [ "));
        Serial.print(gyroQuartiles->getIqrPitch(), 4);
        Serial.print(F(", "));
        Serial.print(gyroQuartiles->getIqrRoll(), 4);
        Serial.print(F(" ]"));
        Serial.print(F(" allowed: ["));
        Serial.print(gyroQuartiles->getMinIqrPitch(), 4);
        Serial.print(F(", "));
        Serial.print(gyroQuartiles->getMaxIqrPitch(), 4);
        Serial.print(F("] ["));
        Serial.print(gyroQuartiles->getMinIqrRoll(), 4);
        Serial.print(F(", "));
        Serial.print(gyroQuartiles->getMaxIqrRoll(), 4);
        Serial.print(F("]"));
    }
}

void SerialView::printLegPosition(Leg *leg) {
    char *state = NULL;
    leg->legStateToString(state);

    Serial.print(state);
    if (leg->isOnGround()) {
        Serial.print(F(" Ground"));
    }
}

void SerialView::printTitle() {
    Serial.println(F("*************************************"));
    Serial.print(F("Trailer balancer "));
    Serial.print(F(VER));
    Serial.print(F(" "));
    Serial.println(F(RELEASE_DATE));
    Serial.println(F("Copyright bezensek.mitja@gmail.com"));
    Serial.println(F("*************************************"));
    Serial.println();
}

void SerialView::refreshSerialDebug() {
    if (serialDebugTimer.elapsedMs() < SERIAL_DEBUG_REFRESH_MS) {
        return;
    }

    serialDebugTimer.restart();

    TrailerDebugData data;

    data.id = timestamp;
    balancer->balancerStateToString(data.state);

    legs[0].legStateToString(data.legAState);
    legs[1].legStateToString(data.legBState);
    legs[2].legStateToString(data.legCState);
    legs[3].legStateToString(data.legDState);

    float *ypr = gyro->getYPR();
    float pitch = ypr[1];
    float roll = ypr[2];
    data.pitch = pitch;
    data.roll = roll;

    if (gyroQuartiles->hasStats()) {
        data.iqrPitch = gyroQuartiles->getIqrPitch();
        data.iqrRoll = gyroQuartiles->getIqrRoll();
    }

    serialDebugWriter.write(&data);
}

