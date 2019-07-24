void setup()
{
    Serial.begin(9600);
}

void loop()
{

    while (Serial.available())
    {
        String a = Serial.readString();
        if (a == "AT")
        {
            Serial.println("OK");
        }
    }

    delay(20);
}
