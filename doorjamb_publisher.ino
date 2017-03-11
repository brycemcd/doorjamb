#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

const char* ssid     = "lolpackets-2.4G";
const char* password = "BryceRules";
const char* mqtt_server = "spark4.thedevranch.net";
const char* doorjamb_topic = "doorjamb";

int sensorPin = A0;
int ledPin = 13;
int sensorValue = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
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
}

void loop() {

  if (!client.connected()) {
    reconnect();
  } else {
      sensorValue = analogRead(sensorPin);
      
      // RESET JSON
      StaticJsonBuffer<200> jsonBuffer; // increase the value of 200 if the json string gets larger
      JsonObject& json = jsonBuffer.createObject();

      json["sensorValue"] = sensorValue;
      
      char postable[1024];
      json.printTo(postable, sizeof(postable));
      
      // Once connected, publish an announcement...
      digitalWrite(ledPin, HIGH);
      delay(500);
      
      client.publish(doorjamb_topic, postable );
      digitalWrite(ledPin, LOW);
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
