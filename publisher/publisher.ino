/*
 * Reads photoresistor and magnetic read switch
 * Publishes json to queue with status of ambient light
 * in the room and status of door (open or closed)
 */
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

const char* ssid     = "iot-2.4G";
const char* password = "i0tconnect";
const char* mqtt_server = "mqtt01.thedevranch.net";
const char* doorjamb_topic = "doorjamb";

const int sensorPin = A0;
const int switchPin = 10;
const int ledPin = 13;
int lightValue = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT);
  //digitalWrite(switchPin, HIGH);

  /* NETWORK SET UP */
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.enableAP(0);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
}

void loop() {
  // RESET JSON
  StaticJsonBuffer<200> jsonBuffer; // increase the value of 200 if the json string gets larger
  JsonObject& json = jsonBuffer.createObject();

  if (!client.connected()) {
    reconnect();
  } else {
    if (digitalRead(switchPin) == HIGH) {
      Serial.println("Door closed");
      json["doorValue"] = "closed";
    } else {
      Serial.println("Door open");
      json["doorValue"] = "open";
    }

    lightValue = analogRead(sensorPin);

    json["lightValue"] = lightValue;

    char postable[1024];
    json.printTo(postable, sizeof(postable));

    json.prettyPrintTo(Serial);
    
    client.publish(doorjamb_topic, postable );
  }
  client.loop();
  delay(1000); // wait 1s before publishing another value
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("doorjambPUB")) {
      Serial.println("connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
