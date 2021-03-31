# Streaming Server
import socket
import time
import serial

#this is the local ip address of the device running this server
HOST = '192.168.1.44'   
PORT = 50007

#initialize the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)


#check if client is running
while True:
    conn, addr = s.accept()
    print 'Client connection accepted ', addr
    while True:
        try:
            #connect to arduino and send the serial values
            arduino = serial.Serial('/dev/cu.usbmodem14101', 115200, timeout=.1)
            while True:
                data = arduino.readline()[:-2] #the last bit gets rid of the new-line chars
                print 'Server sent:', data
                conn.send(data)
        except socket.error, msg:
            print 'Client connection closed', addr
            break

conn.close()
#closes if no connection
