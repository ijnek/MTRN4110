// Part 2 and 3 - Run a motor at a certain speed and direction, and display the count and distance travelled

// Define the pins for the motor and encoder
#define left_motor_en     9
#define left_motor_dirA   6
#define left_motor_dirB   7
#define left_encoder_pinA 2
#define left_encoder_pinB 3

byte encoder0PinALast;
int duration;//the number of the pulses
boolean Direction;//the rotation direction
int start; // Start and end times
int travel;

void setup() {
  Serial.begin(9600); //Initialize the serial port
  travel = 0;
  EncoderInit();
  MotorInit();
  start = millis();
}

void loop() {
  while(millis() - start < 5000){ // Stay in this loop for 5 seconds
    int curr = duration;
    
    analogWrite(left_motor_en, 255*0.8); // Go full speed
    Serial.print("Pulse:");
    Serial.print(curr);
    Serial.print("\t");

    travel += curr;
    Serial.print("Distance so far: ");
    Serial.println(-travel/(360) * 2 * 3.14 * 0.001); // distance travelled = encoder_count/360 * 2 * pi * r

    duration = 0;
    delay(100);
  }
  analogWrite(left_motor_en, 0); // Go full speed
}

void EncoderInit() {
  Direction = true;
  pinMode(left_encoder_pinB,INPUT);
  attachInterrupt(0, wheelSpeed, CHANGE);
}

void MotorInit() {
  pinMode(left_motor_en, OUTPUT);
  pinMode(left_motor_dirA, OUTPUT);
  digitalWrite(left_motor_dirA, LOW);
  digitalWrite(left_motor_dirB, HIGH);
}

void wheelSpeed()
{
  int Lstate = digitalRead(left_encoder_pinA);
  if((encoder0PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(left_encoder_pinB);
    if(val == LOW && Direction)
    {
      Direction = false; //Reverse
    }
    else if(val == HIGH && !Direction)
    {
      Direction = true;  //Forward
    }
  }
  
  encoder0PinALast = Lstate;

  if(!Direction)  duration++;
  else  duration--;
}
