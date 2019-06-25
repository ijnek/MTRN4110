#include "locomotion.h"

void setup() {
  initEverything();
  Serial.println("Enter a number between 1-5");
}

void loop() {
  int X = 90; // degrees
  int Y = 90; // degrees

  if(Serial.available() > 0) {
    char in = Serial.read(); // Get the number

    // Go to the correct function
    switch(in){
      case '1':
        part1();
        break;
      case '2':
        part2();
        break;
      case '3':
        part3();
        break;
      case '4':
        part4(X);
        break;
      case '5':
        part5(Y);
        break;
    }
    
    Serial.println("Please enter a number between 1-5");
  }
}
