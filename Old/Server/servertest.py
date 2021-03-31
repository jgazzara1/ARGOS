# Streaming Server
import socket
import time
from random import randint

HOST = '192.168.1.44'
PORT = 50007

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)

while True:
    conn, addr = s.accept()
    print 'Client connection accepted ', addr
    while True:
        try:
            data = str(randint(0, 9))
            print 'Server sent:', data
            conn.send(data)    
            time.sleep(1)
        except socket.error, msg:
            print 'Client connection closed', addr
            break

conn.close()
