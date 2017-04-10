/*
 * Notify parents when door has opened
 */

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

const char* ssid     = "iot-2.4G";
const char* password = "i0tconnect";
const char* mqtt_server = "mqtt01.thedevranch.net";
const char* doorjamb_topic = "doorjamb";

#define SENSOR_THRESHOLD_FOR_FLASHING 100
#define DELAY_BETWEEN_FLASHES 100

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH); // turn off the light to start
  Serial.begin(115200);
  /* NETWORK SET UP */
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } 
  client.loop();
  //delay(1000); // wait 1s before publishing another value
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("doorjambSUB02")) {
      client.subscribe(doorjamb_topic);
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& jsonRoot = jsonBuffer.parseObject(payload);
  
  if (!jsonRoot.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  int lightValue = jsonRoot["lightValue"];
  Serial.print("light: ");
  Serial.println(lightValue);

  String doorValue = jsonRoot["doorValue"];
  String openValue = "open";
  Serial.print("door: ");
  Serial.println(doorValue);
  
  // Switch on the LED if an 1 was received as first character
  if (doorValue.equals(openValue) && lightValue < SENSOR_THRESHOLD_FOR_FLASHING) {
   flashLED();
  }
}

void flashLED() {
  int flashTimes = 5;
  while(flashTimes > 0) {
    Serial.print(" ");
    Serial.print(flashTimes);
    digitalWrite(BUILTIN_LED, LOW);
    delay(DELAY_BETWEEN_FLASHES);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(DELAY_BETWEEN_FLASHES);
    flashTimes--;
  }
  Serial.println();
}

