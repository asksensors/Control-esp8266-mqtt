/*
 * Description:  Read ESP8266 actuator command from AskSensors over MQTT
 *  Author: https://asksensors.com, 2020
 *  github: https://github.com/asksensors
 */
 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//TODO: ESP8266 MQTT user config
const char* ssid = ".............."; // Wifi SSID
const char* password = ".............."; // Wifi Password
const char* username = ".............."; // my AskSensors username
const char* subTopic = "actuator/............../.............."; // actuator/username/apiKeyOut

//AskSensors MQTT config
const char* mqtt_server = "mqtt.asksensors.com";
unsigned int mqtt_port = 1883;

WiFiClient askClient;
PubSubClient client(askClient);

void setup() {
  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : Read ESP8266 actuator command from AskSensors over MQTT");
  Serial.println("Set LED as output");
  pinMode(LED_pin, OUTPUT);   // set led as output
  
  Serial.print("********** connecting to WIFI : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("->WiFi connected");
  Serial.println("->IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
    if (!client.connected()) 
    reconnect();
  Serial.print("********** Subscribe to AskSensors actuator topic:");
  Serial.print(subTopic);
  // susbscribe
  client.subscribe(subTopic);
  
}

void loop() {
  client.loop();
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Command received from AskSensors[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println("");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("********** Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", username, "")) {  
      Serial.println("-> MQTT client connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("-> try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
