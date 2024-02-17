// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;
int ldrValue = 0;

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("counter");
AdafruitIO_Feed *ldr = io.feed("ldr");

void setup()
{
    // start the serial connection
    Serial.begin(19200);

    // Set up the LED pins
    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    // wait for serial monitor to open
    while (!Serial)
    {
        Serial.print("Connecting to Adafruit IO");
    }

    // connect to io.adafruit.com
    io.connect();

    // wait for a connection
    while (io.status() < AIO_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    // we are connected
    Serial.println();
    Serial.println(io.statusText());
}

void loop()
{
    // io.run(); is required for all sketches.
    // it should always be present at the top of your loop
    // function. it keeps the client connected to
    // io.adafruit.com, and processes any incoming data.
    io.run();

    // Read the LDR value
    ldrValue = analogRead(LDR_PIN);

    Serial.print("LDR PIN :");
    Serial.print(LDR_PIN);
    Serial.print(" Value : ");
    Serial.println(ldrValue);

    // Light the LED based on the LDR value
    if (ldrValue < 1000)
    {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, LOW);
    }
    else if (ldrValue < 2000)
    {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(RED_LED, LOW);
    }
    else
    {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, HIGH);
    }

    // save count to the 'counter' feed on Adafruit IO
    Serial.print("sending -> ");
    Serial.println(count);
    counter->save(count);

    // // increment the count by 1
    count++;

    // Adafruit IO is rate limited for publishing, so a delay is required in
    // between feed->save events. In this example, we will wait three seconds
    // (1000 milliseconds == 1 second) during each loop.
    delay(3000);
}
