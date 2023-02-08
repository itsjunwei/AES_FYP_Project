import serial
import time
import os

# File cleanup 
if os.path.exists('keyData.txt'):
    os.remove('keyData.txt')
    keyFile = open('keyData.txt', 'a+')
    
if os.path.exists('plaintext.txt'):
    os.remove('plaintext.txt')
    plainFile = open('plaintext.txt', 'a+')
    
if os.path.exists('ciphertext.txt'):
    os.remove('ciphertext.txt')
    cipherFile = open('ciphertext.txt', 'a+')

# Open Serial Port
serialcomm = serial.Serial('COM5', 9600, timeout=.1)
serialcomm.flush()

# For testing purposes
if os.path.exists('data.txt'):
    os.remove('data.txt')
    testFile = open('data.txt', 'a+')

# Time checker
startTime = time.time()


## MAIN FUNCTION 
for counter in range(0,1001):
    try:
        
        # Randomly generate a string of 16 bytes
        i = os.urandom(16)

        # Send data to Arduino
        serialcomm.write(i)
        
        print(f'Encrypted {counter} times')
        
        continue
        # # The data will be sent in order of Key, Plaintext, Ciphertext
        # for i in range(0,3):
        #     # Read lines sent by Arduino
        #     lineSent = serialcomm.readline()
        #     # Decode the lines sent
        #     message = lineSent.decode('ascii')
            
        #     if i == 0:
        #         keyFile.write(message.strip())
        #         keyFile.write('\n')
        #     if i == 1:
        #         plainFile.write(message.strip())
        #         plainFile.write('\n')
        #     if i == 2:
        #         cipherFile.write(message.strip())
        #         cipherFile.write('\n')
        
        # print(f"Encrypted {counter} times")
        # continue
        # # Read lines sent by Arduino
        # lineSent = serialcomm.readline()
        # # Decode the lines sent
        # message = lineSent.decode('ascii')
        
        
        # if message.startswith('K '):
        #     keyFile.write(message.strip())
        #     keyFile.write('\n')
            
        # if message.startswith('P '):
        #     plainFile.write(message.strip())
        #     plainFile.write('\n')
            
        # if message.startswith('C '):
        #     cipherFile.write(message.strip())
        #     cipherFile.write('\n')
        
        
        # # If EOF , stop reading
        # if message == "\n":
        #     print("\nEnd Encryption")
        #     continue
        # else:
        #     # Else, output lines to console
        #     print(message.strip())





    except KeyboardInterrupt: # control C
        i = 'off'
        serialcomm.write(i.encode())
        print("\nEncryption Terminated")
        
        # Close the serial port
        serialcomm.close()
        break

serialcomm.close()

# Print the time taken for the entire loop to run
timeTaken = time.time() - startTime()
print(f'Time taken: {timeTaken}')

allLines = serialcomm.readlines()

for line in allLines:
    print(line)
    testFile.write(line.strip())
    testFile.write('\n')
############ TO DO ##################

# 1. Rework the AES Encryption to work byte by byte instead of 16 byte block sequence
# 2. Make the key random 

    