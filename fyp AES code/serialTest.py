import serial
import time
import os
import random
import osc_library
import numpy
import pandas as pd
import shutil


def clearFiles():
    # Clear Screen
    os.system('cls')
    
    if os.path.exists('./data/keyData.txt'):
        os.remove('./data/keyData.txt')
        
    if os.path.exists('./data/plaintext.txt'):
        os.remove('./data/plaintext.txt')
        
    if os.path.exists('./data/ciphertext.txt'):
        os.remove('./data/ciphertext.txt')
        
    shutil.rmtree('./data/traceLines')
    shutil.rmtree('./data/cleanedTraceLines')
    os.mkdir('./data/traceLines')
    os.mkdir('./data/cleanedTraceLines')
        
        

def main():
    
    ######################## SETUP ##############################
    
    # Osciloscope Set-up
    OSC_IP_ADDRESS = '192.168.0.196'
    CHANNEL = 'C1'
    lecroy_if = osc_library.Lecroy(ip_address=OSC_IP_ADDRESS)
    lecroy_if.prepare_for_trace_capture()
    lecroy_if.wait_lecroy()

    # Store all the collected data
    full_array = []

    # How many traces to collect
    loopCount = 50000

    # Files to collect plaintext and ciphertext
    plainFile = open('./data/plaintext.txt', 'a+')
    cipherFile = open('./data/ciphertext.txt', 'a+')    

    # Open Serial Port
    serialcomm = serial.Serial('COM5', 9600)
    serialcomm.timeout = 1

    

    #################### Encryption ##########################


    # Encrypt and Extract Plaintext and Ciphertext            
    for counter in range(loopCount):
        try:

            # Randomly generate a string of 16 bytes
            i = random.randbytes(16)
            i += b'\n'      # Add newline char to terminate string for Arduino

            # Send data to Arduino
            serialcomm.write(i)
            serialcomm.flush()

            while True:
                
                eachLine = serialcomm.read_until()
                message = eachLine.decode()
                
                # End of encryption cycle
                if message == '\n':
                    ##### Collect one trace #####
                    _, interpreted_format = lecroy_if.get_native_signal_float(CHANNEL)
                    thisArray = numpy.asarray(interpreted_format[:])
                    full_array.append(thisArray)
                    
                    print(f'Encrypted {counter+1} times')
                    break
                
                # Collect data
                else:
                    if message.startswith("P"):
                        plainFile.write(message.strip()[2:])
                        plainFile.write('\n')
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

    # Export Trace Raw Data
    for arrayCount in range(loopCount):
        with open(f'./data/traceLines/trace_{arrayCount+1}.csv', 'w') as myFile:
            numpy.savetxt(myFile, full_array[arrayCount], newline=',')
            print(f'Arrays Processed: {arrayCount+1}')
        myFile.close()
    print("Array Data exported to file")

    # Clean the individual traces for processing
    for traceLine in range(loopCount):
        with open(f'./data/traceLines/trace_{traceLine+1}.csv', 'r') as myFile:
            for line in myFile.readlines():
                array = line.split(',')
                array = array[:len(array)-1]

                # Need to convert the values to float
                for idx, element in enumerate(array):
                    array[idx] = float(element)
                
                df = pd.DataFrame(array)
                df.to_csv(f'./data/cleanedTraceLines/trace_{traceLine+1}.csv')
        myFile.close()
        print(f'Cleaned {traceLine+1} files')
    print("Trace Data cleaned for processing")

    
    
if __name__ == "__main__":
    
    # Cleanup workspace
    clearFiles()
    
    # Start Time
    startTime = time.time()
    
    # Encryption Function
    main()
    
    # Print the time taken for the entire loop to run
    timeTaken = time.time() - startTime
    print("Time Taken: {:.2f} seconds".format(timeTaken))