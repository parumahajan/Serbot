#include <SPI.h>
#include <MFRC522.h>

// Define pins for the RC522 RFID module
#define SS_PIN 5   // SDA
#define RST_PIN 4  // RST

#define LED_PIN 2  // Define the onboard LED pin (usually GPIO 2 for ESP32)

MFRC522 rfid(SS_PIN, RST_PIN); // Create an instance of the MFRC522 class

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  while (!Serial) {
    // Wait for the serial connection to be established (if needed)
  }

  // Initialize SPI bus
  SPI.begin();
  
  // Initialize the RFID reader
  rfid.PCD_Init();
  Serial.println("RFID reader initialized. Scan your RFID tag/card...");

  // Set up the LED pin
  pinMode(LED_PIN, OUTPUT);  // Define the LED pin as output
  digitalWrite(LED_PIN, LOW); // Ensure the LED is off initially
}

void loop() {
  // Look for a new RFID tag/card
  if (!rfid.PICC_IsNewCardPresent()) {
    // If no card is detected, continue the loop
    return;
  }
  
  // Attempt to read the detected card's serial number (UID)
  if (!rfid.PICC_ReadCardSerial()) {
    // If reading fails, continue the loop
    return;
  }
  
  // Print the UID (Unique ID) of the detected card/tag
  Serial.print("Card/Tag Detected! UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Print each byte of the UID in hexadecimal format
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Blink the LED to indicate detection
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  delay(200);                  // Wait for 200 milliseconds
  digitalWrite(LED_PIN, LOW);  // Turn the LED off
  
  // Put the card/tag to an idle state to stop reading the same card repeatedly
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}