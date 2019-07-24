#include <Wire.h>
#include "external_DFRobot_LCD.h"

DFRobot_LCD lcd(16, 2);


void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);

    lcd.init();
}

void loop()
{
    // read from port 1, send to port 0:
    if (Serial1.available())
    {
        String s = Serial1.readString();
        Serial.println(s);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(s);
    }

    delay(20);
}
