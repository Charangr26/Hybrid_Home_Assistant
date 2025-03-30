#include "GroveOfflineSensor.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi & MQTT Settings
const char* ssid = "Rajaseelan";     // 🔹 Replace with your Wi-Fi SSID
const char* password = "charanguru"; // 🔹 Replace with your Wi-Fi Password
const char* mqtt_server = "192.168.0.50";  // 🔹 Replace with Raspberry Pi IP

WiFiClient espClient;
PubSubClient client(espClient);

// VC02 Voice Recognition
#define RX_VC02 D7
#define TX_VC02 D8
SoftwareSerial groveSerial(RX_VC02, TX_VC02);

// Relay Pins
#define RELAY1 D1  
#define RELAY2 D2  
#define RELAY3 D3  
#define RELAY4 D4  

// Function to Connect to Wi-Fi
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// Function to Control Relays
void controlRelay(int relayPin, String command) {
  if (command == "ON") {
    digitalWrite(relayPin, LOW);
  } else if (command == "OFF") {
    digitalWrite(relayPin, HIGH);
  }
}

// MQTT Callback Function
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "relay/1") controlRelay(RELAY1, message);
  if (String(topic) == "relay/2") controlRelay(RELAY2, message);
  if (String(topic) == "relay/3") controlRelay(RELAY3, message);
  if (String(topic) == "relay/4") controlRelay(RELAY4, message);
}

// Reconnect to MQTT
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      client.subscribe("relay/#");
    } else {
      delay(5000);
    }
  }
}

// Setup Function
void setup() {
  Serial.begin(115200);
  
  // Set Relay Pins as Output
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Set Relays OFF initially
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  // Initialize Voice Sensor
  groveSerial.begin(115200);

  // Connect to Wi-Fi & MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// Loop Function
void loop() {
  if (!client.connected()) reconnect();
  client.loop(); 

  // Handle Voice Commands
  uint8_t *voiceData = detectVoiceFromGroveSensor(&groveSerial);
  if (voiceData != NULL) {
    String response = getCommandInString(voiceData);
    Serial.println(response);

    if (response == "ok, turn on the light") controlRelay(RELAY1, "ON");
    if (response == "ok, turn off the light") controlRelay(RELAY1, "OFF");
    if (response == "ok, warm light turn on") controlRelay(RELAY2, "ON");
    if (response == "ok, warm light turn off") controlRelay(RELAY2, "OFF");
    if (response == "ok, low fan") controlRelay(RELAY3, "ON");
    if (response == "ok, high fan") controlRelay(RELAY3, "OFF");
    if (response == "ok, start to fan") controlRelay(RELAY4, "ON");
    if (response == "ok, stop to fan") controlRelay(RELAY4, "OFF");
  }

  delay(100);
}
