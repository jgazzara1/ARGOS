# Streaming Client
import socket
import serial # Module needed for serial communication
import time # Module needed to add delays in the code

HOST = '192.168.1.44'
PORT = 50007

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=.1)
ser.flush()

while True:
    #ser.flush()
    
    data = s.recv(7)
    #time.sleep(.955)
    #1024
    print (repr(data))
    send_string = (repr(data))
    # Send the string. Make sure you encode it before you send it to the Arduino.
    ser.write(bytes(send_string.encode('utf-8')))
   
  # Do nothing for 500 milliseconds (0.5 seconds)
    #time.sleep(.955)
 
  # Receive data from the Arduino
    #receive_string = ser.readline().decode('utf-8').rstrip()
 
  # Print the data received from Arduino to the terminal
    #print(receive_string)

s.close()


#!/usr/bin/env python3
 
# Set the port name and the baud rate. This baud rate should match the
# baud rate set on the Arduino.
# Timeout parameter makes sure that program doesn't get stuck if data isn't
# being received. After 1 second, the function will return with whatever data
# it has. The readline() function will only wait 1 second for a complete line 
# of input.

 
# Get rid of garbage/incomplete data
