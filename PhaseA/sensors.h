#include <Wire.h>
#include "VL6180X.h"
#include <NewPing.h>
#include "external_MPU6050_6Axis_MotionApps20.h"
#include "external_MPU6050.h"
#include "external_MPU6050_I2Cdev.h"
#include "external_MPU6050_helper_3dmath.h"

#define TRIGGER_PIN 25      
#define ECHO_PIN 26
#define MAX_DISTANCE 200
#define OUTPUT_READABLE_YAWPITCHROLL
#define OUTPUT_READABLE_WORLDACCEL

unsigned long start;
unsigned long finish;
unsigned int front;
unsigned int left;
unsigned int right;
unsigned int frontl;
int enablePin1 = 22;   // ========= LEFT LIDAR SENSOR ===========
int enablePin2 = 23;   // ========= RIGHT LIDAR SENSOR ========
int enablePin3 = 24;  // ======= FRONT LIDAR SENSOR ===========
int inByte;
int keyPress;
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
int north=0, east=0, south=0, west=0;

struct IMUStruct {
  float angles[3];
  VectorInt16 acceleration;
};

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

VL6180X lidar1 = VL6180X();
VL6180X lidar2 = VL6180X();
VL6180X lidar3 = VL6180X();
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
MPU6050 mpu;
