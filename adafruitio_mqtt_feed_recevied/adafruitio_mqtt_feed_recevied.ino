
#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"

/************ Global State (you don't need to change this!) ******************/

// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'test' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish test = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test");
Adafruit_MQTT_Publish ldrPublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ldrvalue");
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

#define LED_PIN 12
#define LDR_PIN 32

/*************************** Sketch Code ************************************/
int ldrValue = 0;
bool onoff = false;
// uint32_t x = 0;
// void MQTT_connect();

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  // initialize the Serial Monitor
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set Adafruit IO's root CA
  client.setCACert(adafruitio_root_ca);

  // Scbcribe to feed
  MQTT_subscribe();
}

void loop()
{
  MQTT_connect();
  MQTT_publish();
  LED_state();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    // Check if its the onoff button feed
    if (subscription == &onoffbutton)
    {
      Serial.print(F("On-Off button: "));
      Serial.println((char *)onoffbutton.lastread);

      if (strcmp((char *)onoffbutton.lastread, "ON") == 0)
      {
        onoff = true;
      }
      else if (strcmp((char *)onoffbutton.lastread, "OFF") == 0)
      {
        onoff = false;
      }
    }
  }

  // ping the server to keep the mqtt connection alive
  // if (!mqtt.ping())
  // {
  //   Serial.println(F("MQTT Ping failed."));
  //   Serial.println(F("Disconnect"));
  //   mqtt.disconnect();
  // }
}

void LED_state()
{
  if (onoff)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}

void MQTT_publish()
{
  if (!onoff)
  {
    return;
  }
  // Now we can publish stuff!
  ldrValue = analogRead(LDR_PIN);
  Serial.print(F("Publishing to LDR feed: "));
  Serial.print(ldrValue);
  Serial.print(F(" ..."));
  if (!ldrPublish.publish(ldrValue))
  {
    Serial.println(F("Failed"));
  }
  else
  {
    Serial.println(F("OK!"));
  }
}

void MQTT_subscribe()
{
  // Now we can subscribe to a feed
  Serial.print(F("Subscribing to "));
  Serial.print(AIO_USERNAME "/feeds/onoff");
  Serial.print(" ... ");
  if (!mqtt.subscribe(&onoffbutton))
  {
    Serial.println(F("Failed"));
  }
  Serial.println(F("OK!"));
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }

  Serial.println("MQTT Connected!");
}
