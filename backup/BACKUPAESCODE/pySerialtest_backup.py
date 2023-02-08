import serial
import time
serialcomm = serial.Serial('COM5', 9600)
serialcomm.timeout = 1

try:
    i = input("Enter Input: ").strip()
    f = open('dataFile.txt','a')

    if (i == 'on'):
        while True:
            serialcomm.write(i.encode())
            print(serialcomm.readline().decode('ascii'))
            f.write(serialcomm.readline().decode('ascii'))
            f.close()
            f = open('dataFile.txt','a')



except KeyboardInterrupt: # control C
    i = 'off'
    serialcomm.write(i.encode())
    print("\nEncryption Terminated")
    

serialcomm.close()
    