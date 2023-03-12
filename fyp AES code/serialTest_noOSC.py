import serial
import time
import os
import random
from Crypto.Cipher import AES

# keyString = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F".replace(" ", "")
keyString = "2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C".replace(" ", "")
key = bytearray.fromhex(keyString)
cipher = AES.new(key, AES.MODE_ECB)

# Clear Screen
os.system('cls')

# How many traces to collect
loopCount = 10


# Filepath cleanup 
if os.path.exists('./data/keyData.txt'):
    os.remove('./data/keyData.txt')
    
if os.path.exists('./data/plaintext.txt'):
    os.remove('./data/plaintext.txt')
    
if os.path.exists('./data/ciphertext.txt'):
    os.remove('./data/ciphertext.txt')
    
if os.path.exists('./data/test.txt'):
    os.remove('./data/test.txt')


keyFile = open('./data/keyData.txt', 'a+')
plainFile = open('./data/plaintext.txt', 'a+')
cipherFile = open('./data/ciphertext.txt', 'a+')    
testFile = open('./data/test.txt', 'a+')

# Open Serial Port
serialcomm = serial.Serial('COM5', 9600)
serialcomm.timeout = 1

# Time checker
startTime = time.time()

## MAIN FUNCTION 

# Extract Key
try: 
    i = "key\n"
    serialcomm.write(i.encode())
    serialcomm.flush()
    while True:
        keyLine = serialcomm.read_until()
        keyMessage = keyLine.decode()
        
        if keyMessage == "\n":
            print("Key Extracted")
            break
        else:
            keyFile.write(keyMessage[2:])
except KeyboardInterrupt:
    i = 'off'
    serialcomm.write(i.encode())
    print("\nEncryption Terminated")
    
    # Close the serial port
    serialcomm.close()


# Encrypt and Extract Plaintext and Ciphertext            
for counter in range(loopCount):
    try:

        # Randomly generate a string of 16 bytes
        i = random.randbytes(16)
        
        # Convert the string for processing
        j = i.hex(' ').upper()
        i += b'\n'      # Add newline char to terminate string for Arduino

        # Write the plaintext into a file at this step
        plainFile.write(j)
        plainFile.write('\n')

        # Send data to Arduino
        serialcomm.write(i)
        serialcomm.flush()

        while True:
            
            eachLine = serialcomm.read_until()
            message = eachLine.decode()

            if message == '\n':
                print(f'Encrypted {counter+1} times')
                break
            
            else:
                if message.startswith("P"):
                    testFile.write(message.strip()[2:])
                    testFile.write('\n')
                if message.startswith("C"):
                    cipherFile.write(message.strip()[2:])
                    cipherFile.write('\n')

    except KeyboardInterrupt:
        i = 'off'
        serialcomm.write(i.encode())
        print("\nEncryption Terminated")
        
        # Close the serial port
        serialcomm.close()
        break

# Close the port
i = 'off'
serialcomm.write(i.encode())
serialcomm.flush()
print("\nTerminating Connection...")
serialcomm.close()

plainFile.close()
testFile.close()
cipherFile.close()
keyFile.close()



cipherFile = open('./data/ciphertext.txt', 'r')    
testFile = open('./data/test.txt', 'r')

plainLines = testFile.readlines()
cipherLines = cipherFile.readlines()


for idx, line in enumerate(plainLines):
    hexline = line.replace(" ", "")
    cipherLine = cipherLines[idx].replace(" ", "").lower()
    plain = bytearray.fromhex(hexline)
    ciphertext = cipher.encrypt(plain)
    cipherString = bytes(ciphertext).hex()
    if cipherString.strip() != cipherLine.strip():
        print(f"Line {idx+1} has encryption issue")
        print(cipherString)
        print(cipherLine)
        

testFile.close()
cipherFile.close()

# Print the time taken for the entire loop to run
timeTaken = time.time() - startTime
print("Time Taken: {:2f} seconds".format(timeTaken))