// Include external libraries for AES encryption
#include <AES.h>
#include <Crypto.h>

/**********************************************************************/
/*                                                                    */
/*                              Storage Variables                     */
/*                                                                    */
/**********************************************************************/
String incomingByte;
byte buf[16];
byte plaintext[16];
byte ciphertext[16];
using namespace std;
typedef uint8_t byte; //uint32_t

// Create an object of the AES128 class
AES128 aes128;



/**********************************************************************/
/*                                                                    */
/*                              Key Definition                        */
/*                                                                    */
/**********************************************************************/


byte key[16] = {0x2b, 0x7e, 0x15, 0x16,
                0x28, 0xae, 0xd2, 0xa6,
                0xab, 0xf7, 0x15, 0x88,
                0x09, 0xcf, 0x4f, 0x3c};  //AES-128

// byte key[16] = {0x00, 0x01, 0x02, 0x03, 0x04,
//                 0x05, 0x06, 0x07, 0x08,
//                 0x09, 0x0A, 0x0B, 0x0C,
//                 0x0D, 0x0E, 0x0F}; // AES-128 Experimental Key

// byte key[32] = {0x01, 0x02, 0x03, 0x04,
//                 0x05, 0x06, 0x07, 0x08,
//                 0x09, 0x0A, 0x0B, 0x0C,
//                 0x0D, 0x0E, 0x0F, 0x10,
//                 0x11, 0x12, 0x13, 0x14,
//                 0x15, 0x16, 0x17, 0x18,
//                 0x19, 0x1A, 0x1B, 0x1C,
//                 0x1D, 0x1E, 0x1F, 0x20}; // AES-256 Experimental Key

// byte key[32] = {0x52, 0x66, 0x55, 0x6a,
//                 0x58, 0x6e, 0x32, 0x72,
//                 0x35, 0x75, 0x37, 0x78,
//                 0x21, 0x41, 0x25, 0x44,
//                 0x2a, 0x47, 0x2d, 0x4b,
//                 0x61, 0x50, 0x64, 0x53,
//                 0x67, 0x56, 0x6b, 0x59,
//                 0x70, 0x33, 0x53, 0x56
//                }; //AES-256

void setup() {
    Serial.begin(9600);
    // initialize digital pin 11 as an output
    pinMode(11, OUTPUT);
    // Setting the key for AES
    aes128.setKey(key, 16);
}

void loop() {
    // Wait until data is available
    while (Serial.available() == 0){
    }

    // Reading incoming string
    incomingByte = Serial.readStringUntil('\n');

    // Terminate Encryption
    if (incomingByte == "off"){
        // Encryption Ends on Pin 11 LOW
        digitalWrite(11, LOW);
    }

    // General Encryption
    else{

        // Convert String into Byte Array
        incomingByte.getBytes(buf, 17);

        // Extract Plaintext
       for (int i = 0; i<16; i++){
           plaintext[i] = buf[i];
       }
        Serial.print('P');
        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", plaintext[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');
        delay(20);
        noInterrupts();
        

        // Encryption Starts on Pin 11 HIGH
        digitalWrite(11, HIGH);

        aes128.encryptBlock(ciphertext, plaintext);

        // Encryption Ends on Pin 11 LOW
        digitalWrite(11, LOW);
        
        interrupts();
        delay(100);
        
        // Print Ciphertext
        Serial.print('C');
        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", ciphertext[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');
        Serial.flush();
        Serial.println('\n');
    }

}
