int touchPin = 14;
int redLedPin = 33;
int yellowLedPin = 15;
int greenLedPin = 32;

void setup()
{
  Serial.begin(19200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");

  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop()
{
  Serial.println(touchRead(14)); // get value of Touch 0 pin = GPIO 4

  if (isTouch())
  {
    hasTouch();
  }
  else
  {
    resetLed();
  }

  delay(1000);
}

bool isTouch()
{
  return touchRead(14) < 40;
}

void hasTouch()
{
  digitalWrite(redLedPin, HIGH);
  delay(1000);
  digitalWrite(yellowLedPin, HIGH);
  delay(1000);
  digitalWrite(greenLedPin, HIGH);
  delay(1000);
}

void resetLed()
{
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
}
