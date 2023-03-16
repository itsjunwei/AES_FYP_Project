
String incomingByte;
byte buf[16];
byte plaintext[16];
using namespace std;
typedef uint8_t byte; //uint32_t word

/**********************************************************************/
/*                                                                    */
/*                              Key Definition                        */
/*                                                                    */
/**********************************************************************/
byte state[16];
byte key[16];

byte secretKey[16] = {0x2b, 0x7e, 0x15, 0x16,
                    0x28, 0xae, 0xd2, 0xa6,
                    0xab, 0xf7, 0x15, 0x88,
                    0x09, 0xcf, 0x4f, 0x3c};  //AES-128

byte codekey[16] = {0x2b, 0x7e, 0x15, 0x16,
                0x28, 0xae, 0xd2, 0xa6,
                0xab, 0xf7, 0x15, 0x88,
                0x09, 0xcf, 0x4f, 0x3c};  //AES-128

// byte key[16] = {0x01, 0x02, 0x03, 0x04,
//                 0x05, 0x06, 0x07, 0x08,
//                 0x09, 0x0A, 0x0B, 0x0C,
//                 0x0D, 0x0E, 0x0F, 0x10}; // AES-128 Experimental Key

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

// Setup Code
void setup() {
    Serial.begin(9600);
    // initialize digital pin 11 as an output
    pinMode(11, OUTPUT);
}

//Nr-The number of round encrypted (AES-128 10 rounds, AES-192 12 rounds, AES-256 14 rounds)
//Nk-The number of 32-bit words contained in the key (AES-128 4, AES-192 6, AES-256 8)
const int Nr = 10;
const int Nk = 4;


byte S_Box[256] = {
    //   0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F   
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
  };

// byte S_Box[16][16] = {
//   {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
//   {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
//   {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
//   {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
//   {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
//   {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
//   {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
//   {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
//   {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
//   {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
//   {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
//   {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
//   {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
//   {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
//   {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
//   {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
// };


//Round constant, used in key expansion. (AES-128 10 rounds, AES-192 12 rounds, AES-256 14 rounds)

// AES-128 needs all 10 Rcon values
// AES-256 needs 7 Rcon values
                  
byte Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 
                0x20, 0x40, 0x80, 0x1b, 0x36
                     };



/**********************************************************************/
/*                                                                    */
/*                     AES Algorithmic Implementation                 */
/*                                                                    */
/**********************************************************************/

/*****************************************Encryption Process *********************************************/
//Performing the encryption of the plaintext block using 4 different transformation in the initial round,
//the 9/11/13 main rounds and the final round
/*********************************************************************************************************/
/*
    1. SubBytes - The first(upper) 4 bits are row numbers, the last(lower) 4 bits are column numbers
*/
void SubBytes(byte state[16])  {
    for (int i = 0; i < 16; i++)
    {
        state[i] = pgm_read_byte(S_Box + state[i]);
    }
}

/*
    2. ShiftRows - Byte Cyclic Shift

    Before              After
    0 4  8 12      0  4  8 12    
    1 5  9 13  =>  5  9 13  1    
    2 6 10 14     10 14  2  6   
    3 7 11 15     15 3   7 11
*/

void ShiftRows(byte state[16])
{
    byte temp;

    // Row 2
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13]; 
    state[13] = temp;
    
    // Row 3
    temp = state[10]; 
    state[10] = state[2]; 
    state[2] = temp;
    temp = state[14];
    state[14] = state[6]; 
    state[6] = temp;

    // Row 4
    temp = state[3]; 
    state[3] = state[15]; 
    state[15] = state[11]; 
    state[11] = state[7]; 
    state[7] = temp;
   
}

/**
    Multiplication over Finite Fields GF(2^8)
*/

byte xtime(byte x){
    return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x<<1);
}

/**
    3. MixColumns - Column transformation
*/
void MixColumns(byte state[16])
{
    byte i;
    byte a;
    byte b;
    byte c;
    byte d;
    byte e;

    /* Process a column at a time */
    for(i = 0; i < 16; i+=4) {
        a = state[i]; b = state[i+1]; c = state[i+2]; d = state[i+3];
        e = a ^ b ^ c ^ d;
        state[i]   ^= e ^ xtime(a^b);
        state[i+1] ^= e ^ xtime(b^c);
        state[i+2] ^= e ^ xtime(c^d);
        state[i+3] ^= e ^ xtime(d^a);
    }

}

/**
    4.AddRoundKey - XOR each column with the extended key
*/
void AddRoundKey(byte state[16], byte key[16])
{
    for (int i = 0; i < 16; i ++){
        state[i] ^= key[i];
    }
}

void initKey(){
    for (int i = 0; i < 16; i++){
        key[i] = codekey[i];
    }
}

void computeKeyRound(int round){
    for (int i = 0; i < round; i++){
        computeKey(pgm_read_byte(Rcon+i));
    }
}

void computeKey(byte r){
    byte buf0;
    byte buf1;
    byte buf2;
    byte buf3;
    buf0 = pgm_read_byte(S_Box + key[13]);
    buf1 = pgm_read_byte(S_Box + key[14]);
    buf2 = pgm_read_byte(S_Box + key[15]);
    buf3 = pgm_read_byte(S_Box + key[12]);

    key[0] ^= buf0 ^ r;
    key[1] ^= buf1;
    key[2] ^= buf2;
    key[3] ^= buf3;

    key[4] ^= key[0];
    key[5] ^= key[1];
    key[6] ^= key[2];
    key[7] ^= key[3];

    key[8]  ^= key[4];
    key[9]  ^= key[5];
    key[10] ^= key[6];
    key[11] ^= key[7];

    key[12] ^= key[8];
    key[13] ^= key[9];
    key[14] ^= key[10];
    key[15] ^= key[11];
}

void AES128_Encrypt(byte message[16]){

    // Copy Plaintext into state array
    byte state[16];
    for (int j = 0; j<16; j++){
        state[j] = message[j];
    }

    // Initial AddRoundKey step
    initKey();

    delay(0.01);
    noInterrupts();
    digitalWrite(11, HIGH);

    AddRoundKey(state, key);

    // First 9 Rounds
    for (int i = 0; i < 9; i++){
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        computeKey(pgm_read_byte(Rcon+i));
        AddRoundKey(state, key);
    }

    // Last Round has no MixColumns step
    SubBytes(state);
    ShiftRows(state);
    computeKey(pgm_read_byte(Rcon + 9));
    AddRoundKey(state, key);

    digitalWrite(11, LOW);
    interrupts();
    delay(0.01);
    for (int k = 0; k<16; k++){
        message[k] = state[k];
    }
}



// MAIN FUNCTION LOOP

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

    // Extract Key
    if (incomingByte == "key"){

        // Print the Key
        Serial.print('K');
        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", secretKey[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');
        Serial.println('\n');
    }

    // General Encryption
    else{

        // Convert String into Byte Array
        incomingByte.getBytes(buf, 17);

        // Extract Plaintext
       for (int i = 0; i<16; i++){
           plaintext[i] = buf[i];
       }
//        Serial.print('P');
//        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", plaintext[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');

        delay(10);

        // Encryption Starts on Pin 11 HIGH
        // digitalWrite(11, HIGH);
        // delay(0.005);
     
        
        // Perform Encryption on 16 Bytes plaintext
        AES128_Encrypt(plaintext);

        // Encryption Ends on Pin 11 LOW
        // digitalWrite(11, LOW);
        delay(100);

        // Print Ciphertext
//        Serial.print('C');
//        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", plaintext[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');
//        Serial.println('\n');
        Serial.flush();
    }
} 