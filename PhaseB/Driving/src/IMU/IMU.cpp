#include "IMU.h"
#include "Arduino.h"
#include "../MathUtil/MathUtil.h"
#include <Wire.h>
#define MPU_addr 0x68    // I2C address of the MPU-6050
#define GYRO_SCALE 131.0 // gotta scale the gyro, see explanation here (https://electronics.stackexchange.com/questions/39714/how-to-read-a-gyro-accelerometer)
#define GYRO_OFFSET 20 // offset to use for calibration (gyroz)

int16_t GyZ;

IMU::IMU(Blackboard &blackboard)
  : blackboard(blackboard), firstTime(true)
{
}

void IMU::setup()
{
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0);    // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

void IMU::tick()
{
    if (firstTime)
    {
        setup();
        firstTime = false;
    }

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x47); // starting with register   0x47 (GYRO_ZOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 2, true);          // request a total of 2 registers
    int16_t GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    blackboard.gyroZ = DEG2RAD((GyZ + GYRO_OFFSET) / GYRO_SCALE);
}