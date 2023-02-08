#include <aes.h>


AES aes;

const byte key[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

void setup() {
  // Initialize AES with key
  aes.set_key(key, sizeof(key));
  Serial.begin(9600);
}

void loop() {
  String input = "";
  if (Serial.available() > 0) {
    input = Serial.readString();
  }

  // Encrypt input
  byte inputBytes[input.length()];
  input.getBytes(inputBytes, input.length());
  byte outputBytes[input.length()];
  aes.encrypt(inputBytes, outputBytes);
  
  // Send output to serial
  for (int i = 0; i < input.length(); i++) {
    Serial.write(outputBytes[i]);
  }
}
