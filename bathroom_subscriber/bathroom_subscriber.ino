#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

const char* ssid     = "iot-2.4G";
const char* password = "i0tconnect";
const char* mqtt_server = "mqtt01.thedevranch.net";
const char* doorjamb_topic = "doorjamb";

const int sensorThresholdForFlashing = 100;
const int delayBetweenFlashes = 100;

const String DOOR_OPEN = "open";

WiFiClient espClient;
PubSubClient client(espClient);

int ledPin1 = 5;
int ledPin2 = 4;
int ledPin3 = 0;
void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  
  digitalWrite(ledPin1, HIGH); // turn off the light to start
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  Serial.begin(115200);
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
    if (client.connect("doorjambSUB")) {
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
  String doorValue = jsonRoot["doorValue"];
  
  Serial.print("lightValue: ");
  Serial.println(lightValue);

  Serial.print("doorValue: ");
  Serial.println(doorValue);
  
  // Switch on the LED if an 1 was received as first character
  //if (jsonSensorValue > sensorThresholdForFlashing) {  
  if (doorValue == DOOR_OPEN && lightValue < sensorThresholdForFlashing) {  
   illuminate();
  } else {
    turnOffLEDS();
  }
}

void illuminate() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
}

void turnOffLEDS() {
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
}

