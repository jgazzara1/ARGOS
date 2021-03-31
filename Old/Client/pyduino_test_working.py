# Streaming Client
import socket
import serial # Module needed for serial communication
import time # Module needed to add delays in the code

#this is the local ip address of the device running the server
HOST = '192.168.1.44'
PORT = 50007

#initialize the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
#this must be the arduino usb address
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=.1)
ser.flush()

while True:

    #receives data from server and makes it a printable representation
    data = s.recv(7)
    print (repr(data))
    send_string = (repr(data))
    
    # Send the string. Make sure you encode it before you send it to the Arduino.
    ser.write(bytes(send_string.encode('utf-8')))
   

s.close()
