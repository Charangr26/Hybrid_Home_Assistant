#include <SoftwareSerial.h>

#define VC02_RX D7  // GPIO13 (ESP8266 receives data from VC02)
#define VC02_TX D8  // GPIO15 (ESP8266 sends data to VC02)

SoftwareSerial vc02Serial(VC02_RX, VC02_TX);

void setup() {
    Serial.begin(115200);   // Debug output in Serial Monitor
    vc02Serial.begin(9600); // VC02 works at 9600 baud

    Serial.println("Initializing VC02 Auto Start...");

    delay(2000); // Wait for module to stabilize

    Serial.println("Sending Auto Start Command...");
    byte autoStartCmd[] = {0xAA, 0x21, 0xCC}; // Enable Auto Mode
    vc02Serial.write(autoStartCmd, sizeof(autoStartCmd));
}

void loop() {
    if (vc02Serial.available()) {
        Serial.print("Voice Command Received: ");
        while (vc02Serial.available()) {
            Serial.print(vc02Serial.read(), HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}
