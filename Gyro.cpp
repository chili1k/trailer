#include "Gyro.h"
#include "Arduino.h"
#include "Config.h"
#include "Debug.h"

// Unstable i2c:
// https://forum.arduino.cc/index.php?topic=408851.15
// https://github.com/wmarkow/Arduino/tree/afc25a64316e38f29cf887f546b8a09c73375838/hardware/arduino/avr/libraries/Wire/src
// C:\Users\Pag_mini\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.21\libraries\Wire\src

// Increase if gyro can't keep up
// https://github.com/jrowberg/i2cdevlib/issues/408
// Lowers DMP rate
#define MPU6050_DMP_FIFO_RATE_DIVISOR 0x05

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "util/Timer.h"
#include <math.h>

// Minimum angle where the dimension is considered stable
#define MIN_STABLE_ANGLE 0.0011

// If too much time between loops reset FIFO
#define RESET_FIFO_TIMEOUT 100

MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

Timer loopTimer;

Gyro::Gyro() {
    pitch = -1;
    roll = -1;
}

void dmpDataReady() {
    mpuInterrupt = true;
}

void Gyro::setup() {
    Wire.setTimeoutInMillis((uint8_t) 500);

    Wire.begin();
    // 400kbits can be too fast
    //  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    Wire.setClock(100000);

    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(GYRO_INTERRUPT_PIN, INPUT);

    if (!mpu.testConnection()) {
        Serial.println(F("MPU6050 connection failed"));
        return;
    }

    Serial.println(F("MPU6050 connection successful"));

    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // sensor on testboard
    // calibration face up GYRO_XY_ORIENTATION LEG_B
    /*
       mpu.setXAccelOffset(-5530);
       mpu.setYAccelOffset(-1940);
       mpu.setZAccelOffset(1380);
       mpu.setXGyroOffset(-19);
       mpu.setYGyroOffset(1);
       mpu.setZGyroOffset(37);
       */
    // sensor on trailer with duct tape
    // calibration face down GYRO_XY_ORIENTATION LEG_B
    /*
       mpu.setXAccelOffset(-5401);
       mpu.setYAccelOffset(-1805);
       mpu.setZAccelOffset(5229);
       mpu.setXGyroOffset(-14);
       mpu.setYGyroOffset(-3);
       mpu.setZGyroOffset(65);
       */

    // sensor under trailer
    // calibration face down GYRO_XY_ORIENTATION LEG_B
    /*
       Sensor readings with offsets:   -7      0       16375   -1      0       0
       Your offsets:   -5495   -1745   5112    -20     12      8
       new::::::::
       Sensor readings with offsets:  6 -5  16383 0 -1  0
       Your offsets: -256  -6055 5701  -36 114 -16

*/
    mpu.setXAccelOffset(-256);
    mpu.setYAccelOffset(-6055);
    mpu.setZAccelOffset(5701);
    mpu.setXGyroOffset(-36);
    mpu.setYGyroOffset(114);
    mpu.setZGyroOffset(-16);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(GYRO_INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    loopTimer.start();
}

void Gyro::readGyro() {
    mpuInterrupt = false;
    uint16_t fifoCount = mpu.getFIFOCount();

    if (fifoCount == 0) {

    } else if (fifoCount == 1024) {
        Serial.println(F("FIFO overflow"));
        mpu.resetFIFO();
    } else {
        if (!fifoCount || (fifoCount % packetSize != 0)) {
            DPRINT(F("Packet is corrupted. Fifo count: "));
            DPRINTLN(fifoCount);
            mpu.resetFIFO();
        } else {
            unsigned long tmStartRead = millis();
            while (fifoCount >= packetSize) {
                //DPRINT(F("MPU get fifo bytest start"));
                mpu.getFIFOBytes(fifoBuffer, packetSize);
                //DPRINTLN(F(" / end"));
                fifoCount -= packetSize;
            }

            if (millis() - tmStartRead > 200) {
                DPRINTLN(F("I2C failed. Reinitializing ..."));
                Wire.end();
                Wire.begin();
            }

            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            pitch = ypr[1];
            roll = ypr[2];
        }
    }
}

void Gyro::loop() {
    if (!dmpReady || !mpuInterrupt) return;

    if (loopTimer.elapsedMs() > RESET_FIFO_TIMEOUT) {
        DPRINTLN(F("Delay detected. Reset FIFO."));
        mpuInterrupt = false;
        mpu.resetFIFO();
    } else {
        readGyro();
    }

    loopTimer.restart();
}

float *Gyro::getYPR() {
    return ypr;
}

float Gyro::getPitch() {
    return pitch;
}

float Gyro::getRoll() {
    return roll;
}

bool Gyro::isPitchBalanced() {
    return abs(pitch) < MIN_STABLE_ANGLE;
}

bool Gyro::isRollBalanced() {
    return abs(roll) < MIN_STABLE_ANGLE;
}

bool Gyro::isBalanced() {
    bool isBal = isPitchBalanced() && isRollBalanced();
    return isBal;
}
