#include <AESLib.h>
String incomingByte;
byte buf[16];
byte plaintext[16];
using namespace std;
typedef uint8_t byte; //uint32_t

/**********************************************************************/
/*                                                                    */
/*                              Key Definition                        */
/*                                                                    */
/**********************************************************************/


byte key[16] = {0x2b, 0x7e, 0x15, 0x16,
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
                  //  1           2           3           4           5
uint32_t Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                  //  6           7           8           9           10
                     0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000
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
void SubBytes(byte mtx[16])  {
    for (int i = 0; i < 15; i++)
    {
        mtx[i] = S_Box[mtx[i]];  // i = 0, 1, 2... , 15
    }
}

/*
    2. ShiftRows - Byte Cyclic Shift
*/
void ShiftRows(byte mtx[16])
{
    //The second line circle moves one bit to the left
    byte temp = mtx[4];
    for (int i = 0; i < 3; ++i)
        mtx[i + 4] = mtx[i + 5];
    mtx[7] = temp;

    //The third line circle moves two places to the left
    for (int i = 0; i < 2; ++i)
    {
        temp = mtx[i + 8];
        mtx[i + 8] = mtx[i + 10];
        mtx[i + 10] = temp;
    }

    //The fourth line moves three left circles
    temp = mtx[15];
    for (int i = 3; i > 0; --i)
        mtx[i + 12] = mtx[i + 11];
    mtx[12] = temp;

}

/**
    Multiplication over Finite Fields GF(2^8)
*/
byte GFMul(byte a, byte b) {
    byte p = 0;
    byte hi_bit_set;

    for (int counter = 0; counter < 8; counter++) {
        if ((b & byte(1)) != 0) {
            p ^= a;
        }

        hi_bit_set = (byte) (a & byte(0x80));
        a <<= 1;

        if (hi_bit_set != 0) {
            a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
        }
        b >>= 1;
    }

    return p;
}

/**
    3. MixColumns - Column transformation
*/
void MixColumns(byte mtx[4 * 4])
{
    byte arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = mtx[i + j * 4];

        mtx[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
        mtx[i + 4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];
        mtx[i + 8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);
        mtx[i + 12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);
    }
}

/**
    4.AddRoundKey - XOR each column with the extended key
*/
void AddRoundKey(byte mtx[4 * 4], uint32_t k[4])
{
    for (int i = 0; i < 4; ++i)
    {
        uint32_t k1 = k[i] >> 24;
        uint32_t k2 = (k[i] << 8) >> 24;
        uint32_t k3 = (k[i] << 16) >> 24;
        uint32_t k4 = (k[i] << 24) >> 24;

        mtx[i] = mtx[i] ^ byte(k1);
        mtx[i + 4] = mtx[i + 4] ^ byte(k2);
        mtx[i + 8] = mtx[i + 8] ^ byte(k3);
        mtx[i + 12] = mtx[i + 12] ^ byte(k4);
    }
}


/******************************Following is the key extension section ***************************************************************/
/**
   Convert four bytes to one word.
*/
uint32_t Word(byte& k1, byte& k2, byte& k3, byte& k4)
{
    uint32_t result(0x00000000);
    uint32_t temp;
    temp = k1;  // K1
    temp <<= 24;
    result |= temp;
    temp = k2;  // K2
    temp <<= 16;
    result |= temp;
    temp = k3;  // K3
    temp <<= 8;
    result |= temp;
    temp = k4;  // K4
    result |= temp;
    return result;
}

/**
    Cyclic left shift by byte
    That is to say, [a0, a1, a2, a3] becomes [a1, a2, a3, a0]
*/
uint32_t RotWord(uint32_t& rw)
{
    uint32_t high = rw << 8;
    uint32_t low = rw >> 24;
    return high | low;
}

/**
    S-box transformation for each byte in input word
*/
uint32_t SubWord(uint32_t sw)
{
    uint32_t result(0x00000000);
    uint32_t temp;
    int col1 = sw & 0xF;
    int row1 = (sw >> 4) & 0xF;
    int col2 = (sw >> 8) & 0xF;
    int row2 = (sw >> 12) & 0xF;
    int col3 = (sw >> 16) & 0xF;
    int row3 = (sw >> 20) & 0xF;
    int col4 = (sw >> 24) & 0xF;
    int row4 = (sw >> 28) & 0xF;

    byte val1 = S_Box[(row1 * 16 + col1)];
    byte val2 = S_Box[(row2 * 16 + col2)];
    byte val3 = S_Box[(row3 * 16 + col3)];
    byte val4 = S_Box[(row4 * 16 + col4)];

    temp = val4;  // val4
    temp <<= 24;
    result |= temp;
    temp = val3;  // val3
    temp <<= 16;
    result |= temp;
    temp = val2;  // val2
    temp <<= 8;
    result |= temp;
    temp = val1;  // val1
    result |= temp;
    return result;

}


/**
    Key Extension Function - Extended 128-bit key to w[4*(Nr+1)]
*/

//void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)])
// void KeyExpansion(byte key[4 * 4], uint32_t w[4 * (10 + 1)]) // Change Nk / Nr for AES-256
// //  Nr = 10,12,14
// //  Nk = 4 ,6 ,8
// {
//     uint32_t temp;
//     int i = 0;
//     //The first four of w [] are input key s
//     while (i < Nk)
//     {
//         w[i] = Word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
//         ++i;
//     }

//     i = Nk;

//     while (i < 4 * (Nr + 1))
//     {
//         temp = w[i - 1]; //Record the previous word
//         if (i % Nk == 0)
//             w[i] = w[i - Nk] ^ SubWord(RotWord(temp)) ^ Rcon[i / Nk - 1];
//         else
//             w[i] = w[i - Nk] ^ temp;
//         ++i;
//     }
// }

void KeyExpansion(byte key[16], uint32_t w[44])
{
    uint32_t temp;
    int i = 0;

    // Key is copied into the first four words of the expanded key
    for (i=0; i<4; i++){
        w[i] = Word(key[4*i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    }

    // Each added word depends on preceding word
    for (i=4; i<44; i++){
        temp = w[i-1]; // Record previous word
        if (i%4 == 0){
            temp = SubWord(RotWord(temp)) ^ Rcon[i/4];
        }
        else{
            w[i] = w[i-4] ^ temp;
        }
    }
}

/******************************Here are the encryption functions ********************************************************************/
/**
    encryption
*/
//void encrypt(byte in[4*4], word w[4*(Nr+1)])
void encrypt(byte in[16], uint32_t w[44])
//  Nr = 10,12,14
//  Nk = 4,6,8
{
    uint32_t key[4];

    // Add initial RoundKey
    for (int i = 0; i < 4; i++)
        key[i] = w[i];
    AddRoundKey(in, key);

    // Nr - 1 encryption rounds
    for (int round = 1; round < Nr; round++)
    {
        SubBytes(in);
        ShiftRows(in);
        MixColumns(in);
        for (int i = 0; i < 4; i++)
            key[i] = w[4 * round + i];
        AddRoundKey(in, key);
    }

    // Last Round does not do MixColumns
    SubBytes(in);
    ShiftRows(in);
    for (int i = 0; i < 4; i++)
        key[i] = w[4 * Nr + i];
    AddRoundKey(in, key);
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

    // Extract Key
    if (incomingByte == "key"){

        // Print the Key
        Serial.print('K');
        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", key[i]);
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
        Serial.print('P');
        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", plaintext[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');

        // Key Expansion
        uint32_t w[4 * (Nr + 1)];
        KeyExpansion(key, w);
        delay(100);

        // Encryption Starts on Pin 11 HIGH
        digitalWrite(11, HIGH);

        // Perform Encryption on 16 Bytes plaintext
        aes128_enc_single(key, plaintext);

        // Encryption Ends on Pin 11 LOW
        digitalWrite(11, LOW);
        delay(150);

        // Print Ciphertext
        Serial.print('C');
        Serial.print(' ');
        for (int i=0; i<16; i++){
            char charVal[2];
            sprintf(charVal, "%02X", plaintext[i]);
            Serial.print(charVal);
            Serial.print(' ');
        }
        Serial.print('\n');
        Serial.flush();
        Serial.println('\n');
    }
} 
