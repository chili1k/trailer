#include "Gyro.h"
#include "Arduino.h"
#include "Config.h"
#include "Debug.h"

// https://github.com/wmarkow/Arduino/tree/afc25a64316e38f29cf887f546b8a09c73375838/hardware/arduino/avr/libraries/Wire/src

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include <math.h>

// Minimum angle where the dimension is considered stable
#define MIN_STABLE_ANGLE 0.0005

#define INTERRUPT_TIMEOUT 2000

MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

unsigned long lastPrintTime = 0;

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
    return false;
  }

  Serial.println(F("MPU6050 connection successful"));

  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  /*
-491	726	1609	141	0	-6

Sensor readings with offsets:	8	8	16388	0	-2	0
Your offsets:	-5471	-1884	1369	-21	-2	36

Data is printed as: acelX acelY acelZ giroX giroY giroZ
Check that your sensor readings are close to 0 0 16384 0 0 0
If calibration was succesful write down your offsets so you can set them in your projects using something similar to mpu.setXAccelOffset(youroffset)

  */
  
  /*
  mpu.setXAccelOffset(-5471);
  mpu.setYAccelOffset(-1884);
  mpu.setZAccelOffset(1369);
  mpu.setXGyroOffset(-21);
  mpu.setYGyroOffset(-2);
  mpu.setZGyroOffset(36);
  */
  // -5530	-1940	1380	-19	1	37
  mpu.setXAccelOffset(-5530);
  mpu.setYAccelOffset(-1940);
  mpu.setZAccelOffset(1380);
  mpu.setXGyroOffset(-19);
  mpu.setYGyroOffset(1);
  mpu.setZGyroOffset(37);

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
}

void Gyro::readGyro() {
  mpuInterrupt = false;
  //DPRINTLN(F("Get fifo"));
  uint16_t fifoCount = mpu.getFIFOCount();
  //DPRINTLN(F("Got fifo"));

  if (fifoCount == 0) {
    //Serial.println(F("FIFO count 0"));
  } else if (fifoCount == 1024) {
    Serial.println(F("FIFO overflow"));
    //DPRINT(F("Reset fifo start 1"));
    mpu.resetFIFO();
    //DPRINTLN(F(" / end"));
  } else { 
    if (fifoCount % packetSize != 0) {
      Serial.print(F("Packet is corrupted. Fifo count: "));
      Serial.println(fifoCount);
      //DPRINT(F("Reset fifo start 2"));
      mpu.resetFIFO();
      //DPRINTLN(F(" / end"));
    } else {

      unsigned long tmStartRead = millis();
      while (fifoCount >= packetSize) {
        //DPRINT(F("MPU get fifo bytest start"));
        // tu crkne
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        //DPRINTLN(F(" / end"));
        fifoCount -= packetSize;
      }

      if (millis() - tmStartRead > 200) {
        //DPRINTLN(F("I2C failed. Reinitializing ..."));
        Wire.end();
        Wire.begin();
      }

      //DPRINT(F("Q"));
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      //DPRINT(F("G"));
      mpu.dmpGetGravity(&gravity, &q);
      //DPRINT(F("Y"));
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      //DPRINTLN(F("!"));

      pitch = ypr[1];
      roll = ypr[2];
    }
  }
}

void Gyro::loop() {
  if (!dmpReady || !mpuInterrupt) return;
  
  readGyro();

  /*
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
    //printPitchRoll();
  }
  */
}

float *Gyro::getYPR() {
  return ypr;
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
  if (isBal) {
    Serial.print(pitch,4);
    Serial.print(", ");
    Serial.println(roll,4);
    Serial.print("Is pitch balanced: ");
    Serial.println(isPitchBalanced());
    Serial.print("Is roll balanced: ");
    Serial.println(isRollBalanced());
  }
  return isBal;
}
