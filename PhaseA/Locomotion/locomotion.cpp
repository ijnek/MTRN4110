#include "locomotion.h"

byte LPinALast;
byte RPinALast;
int Lcount;
int Rcount;
boolean LDirection;
boolean RDirection;

// Static helper functions
static void EncoderInit();
static void MotorInit();
static void leftWheelSpeed();
static void rightWheelSpeed();
static void setLeftDirection(int dir);
static void setRightDirection(int dir);

void initEverything() {
  Serial.begin(9600); //Initialize the serial port

  // Configure digital pin 13 (the LED)
  hardware::io_mode mode = hardware::io_mode::output;
  hardware::led::config_io_mode(mode);

  // Set up the left encoder
  Lcount = 0;
  LDirection = true;
  pinMode(left_encoder_pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(left_encoder_pinA), leftWheelSpeed, CHANGE);

  // Set up the right encoder
  Rcount = 0;
  RDirection = true;
  pinMode(right_encoder_pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(right_encoder_pinA), rightWheelSpeed, CHANGE);

  // Set up the left motor
  pinMode(left_motor_en, OUTPUT);
  pinMode(left_motor_dirA, OUTPUT);

  // Set up the right motor
  pinMode(right_motor_en, OUTPUT);
  pinMode(right_motor_dirA, OUTPUT);
}

/*
 * Toggle an LED – on for 2 seconds, then off 
 */
void part1(){
  while(true){
    hardware::led::write(hardware::logic_level::high); // Turn LED on
    delay(2000); // Wait for 2 seconds
    hardware::led::write(hardware::logic_level::low); // Turn LED off
    delay(2000);
  }  
}

/*
 * Run motor 1 at full speed in the forward direction for 5 seconds
 * Display the readings of the associated encoder on the serial monitor of Arduino
 * Display the distance travelled by the wheel
 */
void part2() {
  setLeftDirection(1); // Make the left motor go forward
  analogWrite(left_motor_en, 255*0.8); // Run the left motor at full forward speed
  
  float ppcm = 1920 / (PI * 6.5); // pulses_per_cm = pulse_per_rev * pi*d;
  float distance = 0;
  int start = millis();

  while (millis() - start < 5000) { // Stay in this loop for 5 seconds
    int curr_reading = Lcount; // get the current encoder reading
    
    Serial.print("Encoder Reading: ");
    Serial.print(curr_reading);
    Serial.print("\t");

    distance += abs(curr_reading) / ppcm; // distance travelled = encoder_count/pulses_per_cm
    Serial.print("Distance so far: ");
    Serial.print(distance);
    Serial.println(" cm");

    Lcount = 0;
    delay(100);
  }
  analogWrite(left_motor_en, 0); // Stop the motor
}

/*
  Run motor 2 at half speed in the backward direction for 5 seconds
  Display the readings of the associated encoder on the serial monitor of Arduino
  Display the distance travelled by the wheel 
*/
void part3() {
  setRightDirection(0); // Make the right motor go backward
  analogWrite(right_motor_en, 255 * 0.4); // Run the right motor at half backward speed
  
  float ppcm = 1920 / (PI * 6.5); // pulses_per_cm = pulse_per_rev * pi*d;
  float distance = 0;
  int start = millis();

  while (millis() - start < 5000) { // Stay in this loop for 5 seconds
    int curr_reading = Rcount;
    Serial.print("Encoder Reading: ");
    Serial.print(curr_reading);
    Serial.print("\t");

    distance += abs(curr_reading) / ppcm; // distance travelled = encoder_count/pulses_per_cm
    Serial.print("Distance so far: ");
    Serial.print(distance);
    Serial.println(" cm");

    Rcount = 0;
    delay(100);
  }
  analogWrite(right_motor_en, 0); // Stop the motor
}


/*
 *  Control the left wheel to run for an angle (X deg)
 *  Display the readings of the associated encoder on the serial monitor.
 */
void part4(int X){
  Serial.print("Rotating by ");
  Serial.print(X);
  Serial.println(" degrees");
  
  // Pick the direction to rotate the wheel
  X < 0 ? setLeftDirection(0) : setLeftDirection(1);

  /* 
   * Rotate it by a certain degree
   * In 360 deg there are 1920 pulses
   * Therefore, there should be ~5 pulses per degree
   * To move the wheel by X deg (+/- 10%), count to X*5 pulses
   * Using 90% of X as this tends to overshoot
   */
  Lcount = 0;
  int curr_reading = Lcount;
  analogWrite(left_motor_en, 255*0.2);
  while(abs(curr_reading) < abs((X*0.9)*5)) {
    Serial.print("Degree: ");
    Serial.println(curr_reading/5);
    curr_reading = Lcount;
  }
  analogWrite(left_motor_en, 0);
}

/*
 *  Control the right wheel to run for an angle (Y deg)
 *  Display the readings of the associated encoder on the serial monitor.
 */
void part5(int Y){
  Serial.print("Rotating by ");
  Serial.print(Y);
  Serial.println(" degrees");
  
  // Pick the direction to rotate the wheel
  Y < 0 ? setRightDirection(0) : setRightDirection(1);

  Rcount = 0;
  int curr_reading = Rcount;
  analogWrite(right_motor_en, 255*0.2);
  while(abs(curr_reading) < abs((Y*0.9)*5)) {
    Serial.print("Degree: ");
    Serial.println(curr_reading/5);
    curr_reading = Rcount;
  }
  analogWrite(right_motor_en, 0);
}

/*
 * Interrupt service for the left wheel encoder
 */
static void leftWheelSpeed() {
  int curr_state = digitalRead(left_encoder_pinA); // Get the most recent state of the encoder
  
  // Figure out which way the encoder is rotating if the state has changed
  if ((LPinALast == LOW) && curr_state == HIGH) {
    int val = digitalRead(left_encoder_pinB);
    if (val == LOW && LDirection) LDirection = false; //Reverse
    else if (val == HIGH && !LDirection) LDirection = true;  //Forward
  }
  LPinALast = curr_state; // Update the state

  // Increment/Decrement the count
  if (!LDirection)  Lcount++;
  else  Lcount--;
}

/*
 * Interrupt service for the right wheel encoder
 */
static void rightWheelSpeed() {
  int curr_state = digitalRead(right_encoder_pinA); // Get the most recent state of the encoder
  
  // Figure out which way the encoder is rotating if the state has changed
  if ((RPinALast == LOW) && curr_state == HIGH) {
    int val = digitalRead(right_encoder_pinB);
    if (val == LOW && RDirection) RDirection = false; //Reverse
    else if (val == HIGH && !RDirection) RDirection = true;  //Forward
  }
  RPinALast = curr_state; // Update the state

  // Increment/Decrement the count
  if (!RDirection)  Rcount++;
  else  Rcount--;
}

/*
 * Set the direction for the left wheel
 */
static void setLeftDirection(int dir){
  if(dir == 1){ // Go forward
    digitalWrite(left_motor_dirA, HIGH);
    digitalWrite(left_motor_dirB, LOW);
  } else { // Go backwards
    digitalWrite(left_motor_dirA, LOW);
    digitalWrite(left_motor_dirB, HIGH);
  }
}

/*
 * Set the direction for the right wheel
 */
static void setRightDirection(int dir){
  if(dir == 1){ // Go forward
    digitalWrite(right_motor_dirA, HIGH);
    digitalWrite(right_motor_dirB, LOW);
  } else { // Go backwards
    digitalWrite(right_motor_dirA, LOW);
    digitalWrite(right_motor_dirB, HIGH);
  }
}
