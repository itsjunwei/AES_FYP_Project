import serial
import time
import os
import random
import osc_library
import numpy

# Clear Screen
os.system('cls')

# How many traces to collect
loopCount = 1000

# Osciloscope Set-up
OSC_IP_ADDRESS = '192.168.0.196'
CHANNEL = 'C1'
lecroy_if = osc_library.Lecroy(ip_address=OSC_IP_ADDRESS)
lecroy_if.prepare_for_trace_capture()
lecroy_if.wait_lecroy()

# Store all the collected data
full_array = []

# Filepath cleanup 
if os.path.exists('./data/keyData.txt'):
    os.remove('./data/keyData.txt')
    
if os.path.exists('./data/plaintext.txt'):
    os.remove('./data/plaintext.txt')
    
if os.path.exists('./data/ciphertext.txt'):
    os.remove('./data/ciphertext.txt')
    
if os.path.exists('./data/test.txt'):
    os.remove('./data/test.txt')
    
if os.path.exists('./data/traceFile.csv'):
    os.remove('./data/traceFile.csv')

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
            
            # End of decrypt, PIN 11 LOW at this point
            if message == '\n':
                ##### Collect one trace ####
                _, interpreted_format = lecroy_if.get_native_signal_float(CHANNEL)
                thisArray = numpy.asarray(interpreted_format[:])
                full_array.append(thisArray)
                
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

# Export data to external file
arrayCount = 1
with open('./data/traceFile.csv', 'w') as myFile:
    for one_array in full_array:
        numpy.savetxt(myFile, one_array, newline=",")
        myFile.write('\n')
        print(f'Arrays Processed: {arrayCount}')
        arrayCount = arrayCount + 1
print("Array Data exported to file")

# Print the time taken for the entire loop to run
timeTaken = time.time() - startTime
print("Time Taken: {:2f} seconds".format(timeTaken))