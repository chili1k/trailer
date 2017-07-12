#include "Gyro.h"
#include "Arduino.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include <math.h>

#define INTERRUPT_PIN 2
// Minimum angle where the dimension is considered stable
#define MIN_STABLE_ANGLE 0.10

MPU6050 mpu;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

float ypr[3];           // yaw, pitch, roll
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

unsigned long lastPrintTime = 0;

Gyro::Gyro() {
  pitch = -1;
  roll = -1;
}

void dmpDataReady() {
    mpuInterrupt = true;
}

bool Gyro::setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately

  Serial.println(F("aInitializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  if (!mpu.testConnection()) {
    Serial.println(F("MPU6050 connection failed"));
    return false;
  }

  Serial.println(F("MPU6050 connection successful"));

  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  
  mpu.setXAccelOffset(-412);
  mpu.setYAccelOffset(805);
  mpu.setZAccelOffset(1688);
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
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
}

void Gyro::loop() {
  // if programming failed, don't try to do anything
  if (!dmpReady) return;
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) { }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      Serial.println(F("FIFO overflow!"));
      // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
      // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    
      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      // Round to 2 decimals
      pitch = round(ypr[1]*100.0)/100.0;
      roll = round(ypr[2]*100.0)/100.0;
      printPitchRoll();
    }
}

void Gyro::printPitchRoll() {
  unsigned long now = millis();

  if (now - lastPrintTime < 500) {
    return;
  }

  lastPrintTime = millis();

  Serial.print("yaw: ");
  Serial.println(ypr[0]);
  Serial.print("Pitch: ");
  Serial.println(pitch);
  Serial.print("Roll: ");
  Serial.println(roll);

  isBalanced();
}

Position Gyro::getPitchPosition() {
  if (pitch < MIN_STABLE_ANGLE*-1) {
    return UnderBalanced;
  } else if (pitch > MIN_STABLE_ANGLE) {
    return OverBalanced;
  }

  return Balanced;
}

Position Gyro::getRollPosition() {
  if (roll < MIN_STABLE_ANGLE*-1) {
    return UnderBalanced;
  } else if (roll > MIN_STABLE_ANGLE) {
    return OverBalanced;
  }

  return Balanced;
}

bool Gyro::isBalanced() {
//  Serial.println(getRollPosition());
//  Serial.println(getPitchPosition());
  
  bool isBalanced = (getRollPosition() == Balanced && getPitchPosition() == Balanced);
  if (isBalanced) {
    Serial.println("System balanced. Good job!");
  }
  return isBalanced;
}



