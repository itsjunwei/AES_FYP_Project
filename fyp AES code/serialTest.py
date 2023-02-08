import serial
import time
import os

# File cleanup 
if os.path.exists('keyData.txt'):
    os.remove('keyData.txt')
    
if os.path.exists('plaintext.txt'):
    os.remove('plaintext.txt')
    
if os.path.exists('ciphertext.txt'):
    os.remove('ciphertext.txt')

keyFile = open('keyData.txt', 'a+')
plainFile = open('plaintext.txt', 'a+')
cipherFile = open('ciphertext.txt', 'a+')    

# Open Serial Port
serialcomm = serial.Serial('COM5', 9600)
serialcomm.timeout = 10

# Time checker
startTime = time.time()

## MAIN FUNCTION 
for counter in range(0,200):
    try:

        # # Randomly generate a string of 16 bytes
        i = os.urandom(16)
        i += b'\n'

        # Send data to Arduino
        serialcomm.write(i)
        serialcomm.flush()

        print(f'Encrypting {counter+1} times')
        continue

    except KeyboardInterrupt:
        i = 'off'
        serialcomm.write(i.encode())
        print("\nEncryption Terminated")
        
        # Close the serial port
        serialcomm.close()
        break


try:
    print("Fetching Data Now")
    while True:
        
        # read until '\n' char
        eachLine = serialcomm.read_until()
        message = eachLine.decode()
        
        if message == '\n':
            print('End of File')
            break
        
        elif "not" not in message:
            if message.startswith("P"):
                plainFile.write(message[2:])
            elif message.startswith("C"):
                cipherFile.write(message[2:])
            else:
                keyFile.write(message[2:])
        continue
    
except KeyboardInterrupt:
    i = 'off'
    serialcomm.write(i.encode())
    
    # Close the serial port
    serialcomm.close()
    print("\nData Extraction Terminated")

# Close the port
serialcomm.close()

# Print the time taken for the entire loop to run
timeTaken = time.time() - startTime
print("Time Taken: {:2f} seconds".format(timeTaken))