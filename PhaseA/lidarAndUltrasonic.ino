#include "sensors.h"

struct IMUStruct myIMU;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  // set up IMU
  IMUSetup();
  
  // set up enables
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(enablePin3, OUTPUT);
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2,LOW);
  digitalWrite(enablePin3, LOW);
  delay(1000);

  // set up first sensor
  // left lidar, connect to pin 22
  digitalWrite(enablePin1, HIGH);
  delay(50);
  lidar1.init();
  lidar1.configureDefault();
  lidar1.setAddress(0x30); // change to new address
  lidar1.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  lidar1.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  lidar1.setTimeout(500);
  lidar1.stopContinuous();
  lidar1.setScaling(1); // configure range or precision 1, 2 oder 3 mm
  delay(300);
  lidar1.startInterleavedContinuous(100);
  delay(100);
  
  // set up the second LiDAR sensor
  digitalWrite(enablePin2, HIGH);
  delay(50);
  lidar2.init();
  lidar2.configureDefault();
  lidar2.setAddress(0x31); // change to new address
  lidar2.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  lidar2.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  lidar2.setTimeout(500);
  lidar2.stopContinuous();
  lidar2.setScaling(1); // configure range or precision 1, 2 oder 3 mm
  delay(300);
  lidar2.startInterleavedContinuous(100);
  delay(100);

  // set up the third LiDAR sensor
  digitalWrite(enablePin3, HIGH);
  delay(50);
  lidar3.init();
  lidar3.configureDefault();
  lidar3.setAddress(0x32); // change to new address
  lidar3.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  lidar3.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  lidar3.setTimeout(500);
  lidar3.stopContinuous();
  lidar3.setScaling(1); // configure range or precision 1, 2 oder 3 mm
  delay(300);
  lidar3.startInterleavedContinuous(100);
  delay(100);
}

void loop() {
  left = lidar1.readRangeSingleMillimeters();
  right = lidar2.readRangeSingleMillimeters();
//  frontl = lidar3.readRangeSingleMillimeters();
  front = sonar.ping_cm();
  //myIMU = IMULoop();

  if (Serial.available() > 0) {
      inByte = Serial.read();
  }

  if(inByte == '4') keyPress = 4;
  if(inByte == '3') keyPress = 3;
  if(inByte == '2') keyPress = 2;
  if(inByte == '1') keyPress = 1;

  if(keyPress == 1) {
    Serial.print("roll: ");
    Serial.print(myIMU.angles[2] * 180/M_PI);
    Serial.print("\t");
    Serial.print("pitch: ");
    Serial.print(myIMU.angles[1] * 180/M_PI);
    Serial.print("\t");
    Serial.print("yaw: ");
    Serial.print(myIMU.angles[0] * 180/M_PI);
    Serial.print("\t");
    Serial.print("X acc: ");
    Serial.print(myIMU.acceleration.x);
    Serial.print("\t");
    Serial.print("Y acc: ");
    Serial.print(myIMU.acceleration.y);
    Serial.print("\t");
    Serial.print("Z acc: ");
    Serial.print(myIMU.acceleration.z);
    Serial.print("\t");
    Serial.print("ultrasonic: ");
    Serial.print(front*10);
    Serial.print("\t");
    Serial.print("Lidar1: ");
    Serial.print(left);
    Serial.print("\t");
    Serial.print("Lidar2: ");
    Serial.print(right);
//    Serial.print("Lidar3: ");
//    Serial.print(frontl);
    Serial.println();
  }

  if(keyPress == 2) { // start functionality
    start = millis();
    while(front > 3 & front < 10)
    {
      front = sonar.ping_cm();
    }
    finish = millis();
    if(finish-start >= 2000)
    {
      Serial.println("Start");
    }
  }

  if(keyPress == 3) { // 3 wall awareness
    if(left > 40 & left < 80) // lidar 1
    {
      Serial.print("1");
    } else {
      Serial.print("0");
    }
    Serial.print("\t");
    if(front > 3 & front < 10) // ultrasonic
    {
      Serial.print("1");
    } else {
      Serial.print("0");
    }
    Serial.print("\t");
    if(right > 40 & right < 80)
    {
      Serial.print("1");
    } else {
      Serial.print("0");
    }
    Serial.println();
  }

  if(keyPress == 4) { // 4 wall awareness

    if(myIMU.angles[0]*180/M_PI > -20 & myIMU.angles[0]*180/M_PI < 20)
    {
      if(front>3 & front<10) north = 1;
      else north = 0; 
      if(left>40 & left<80) west = 1;
      else west = 0;
      if(right>40 & right<80) east = 1;
      else east = 0;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print("?");
      Serial.print("\t");
      Serial.println(west);
    }
    
    // turned by 90 deg
    if(myIMU.angles[0]*180/M_PI > 70 & myIMU.angles[0]*180/M_PI < 110)
    {
      if(front>3 & front<10) west = 1;
      else west = 0;
      if(left>40 & left<80) south = 1;
      else south = 0;
      if(right>40 & right<80) north = 1;
      else north = 0;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print(south);
      Serial.print("\t");
      Serial.println(west);
    }
    // turned by 180 deg
    if(myIMU.angles[0]*180/M_PI > 160 & myIMU.angles[0]*180/M_PI < 200)
    {
      if(front>3 & front<10) south = 1;
      else south = 0;
      if(left>40 & left<80) east = 1;
      else east = 0;
      if(right>40 & right<80) west = 1;
      else west = 1;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print(south);
      Serial.print("\t");
      Serial.println(west);
    }
    // turned by 270 deg
    if(myIMU.angles[0]*180/M_PI > 250 & myIMU.angles[0]*180/M_PI < 290)
    {
      if(front>3 & front<10) east = 1;
      else south = 0;
      if(left>40 & left<80) north = 1;
      else east = 0;
      if(right>40 & right<80) south = 1;
      else west = 1;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print(south);
      Serial.println(west);
    }
    
    // turned by -90 deg
    if(myIMU.angles[0] < -70 & myIMU.angles[0] > -110)
    {
      if(front>3 & front<10) east = 1;
      else south = 0;
      if(left>40 & left<80) north = 1;
      else east = 0;
      if(right>40 & right<80) south = 1;
      else west = 1;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print(south);
      Serial.println(west);
    }
    // turned by -180 deg
    if(myIMU.angles[0] < -160 & myIMU.angles[0] > -200)
    {
      if(front>3 & front<10) south = 1;
      else south = 0;
      if(left>40 & left<80) east = 1;
      else east = 0;
      if(right>40 & right<80) west = 1;
      else west = 1;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print(south);
      Serial.print("\t");
      Serial.println(west);
    }
    // turned by -270 deg
    if(myIMU.angles[0] < -250 & myIMU.angles[0] > -290)
    {
      if(front>3 & front<10) west = 1;
      else west = 0;
      if(left>40 & left<80) south = 1;
      else south = 0;
      if(right>40 & right<80) north = 1;
      else north = 0;
      Serial.print(north);
      Serial.print("\t");
      Serial.print(east);
      Serial.print("\t");
      Serial.print(south);
      Serial.print("\t");
      Serial.println(west);      
    }
  }
  
  delay(100);
}
