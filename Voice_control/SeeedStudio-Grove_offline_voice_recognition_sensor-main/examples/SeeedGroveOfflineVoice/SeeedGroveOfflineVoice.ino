#include "GroveOfflineSensor.h"
#include <SoftwareSerial.h>
#define RX_VC02 D7
#define TX_VC02 D8

// Relay Pins
#define RELAY1 D1  // GPIO5
#define RELAY2 D2  // GPIO4
#define RELAY3 D3  // GPIO0
#define RELAY4 D4  // GPIO2

SoftwareSerial groveSerial(RX_VC02, TX_VC02); // RX, TX

void setup() {
  Serial.begin(115200);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);

  while (!Serial); // wait for serial port to connect. Needed for native USB port only , This port is for displaying data Grove Sensor sends

  groveSerial.begin(115200); // Make sure to set the baud rate to match your communication
    
}

void loop() {
  // Your main code here
   uint8_t *voiceData = detectVoiceFromGroveSensor(&groveSerial);

  if(voiceData != NULL){
    String response = getCommandInString(voiceData);
    Serial.println(response);
    
    if(response == "ok, turn on the light")
    {
      digitalWrite(D1, LOW);
    }

    if(response == "ok, turn off the light")
    {
      digitalWrite(D1, HIGH);
    }

    if(response == "ok, warm light turn on")
    {
      digitalWrite(D2, LOW);
    }

    if(response == "ok, warm light turn off")
    {
      digitalWrite(D2, HIGH);
    }

    if(response == "ok, low fan")
    {
      digitalWrite(D3, LOW);
    }

    if(response == "ok, high fan")
    {
      digitalWrite(D3, HIGH);
    }

        if(response == "ok, start to fan")
    {
      digitalWrite(D4, LOW);
    }

    if(response == "ok, stop to fan")
    {
      digitalWrite(D4, HIGH);
    }
  }

  delay(1000);
}
