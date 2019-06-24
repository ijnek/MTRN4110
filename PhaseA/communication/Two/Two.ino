void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop()
{
    // read from port 1, send to port 0:
    if (Serial1.available())
    {
        Serial.print((char)Serial1.read());
    }

    delay(20);
}
