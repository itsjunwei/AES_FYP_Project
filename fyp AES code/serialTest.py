import serial
import time
import os
import random

# File cleanup 
if os.path.exists('./data/keyData.txt'):
    os.remove('./data/keyData.txt')
    
if os.path.exists('./data/plaintext.txt'):
    os.remove('./data/plaintext.txt')
    
if os.path.exists('./data/ciphertext.txt'):
    os.remove('./data/ciphertext.txt')

keyFile = open('./data/keyData.txt', 'a+')
plainFile = open('./data/plaintext.txt', 'a+')
cipherFile = open('./data/ciphertext.txt', 'a+')    

# Open Serial Port
serialcomm = serial.Serial('COM5', 9600)
serialcomm.timeout = 1

# Loop Counter
loopCount = 2000

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
for counter in range(0,loopCount):
    try:

        # # Randomly generate a string of 16 bytes
        i = random.randbytes(16)
        i += b'\n'      # Add newline char to terminate string for Arduino

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
                    plainFile.write(message.strip()[2:])
                    plainFile.write('\n')
                elif message.startswith("C"):
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

# Print the time taken for the entire loop to run
timeTaken = time.time() - startTime
print("Time Taken: {:2f} seconds".format(timeTaken))