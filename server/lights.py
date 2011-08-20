#!/usr/bin/env python
import serial,sys
import socket

def setLight(r,g,b):
	r_c = chr(r)
	g_c = chr(g)
	b_c = chr(b)
	ser.write(r_c+g_c+b_c)

ser = serial.Serial("/dev/ttyUSB0",19200)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0",54127))

while True:
	data,addr = sock.recvfrom(1024)
	#print "Got data from %s, length %i" % (addr,len(data))
	if len(data) == 3:
		setLight(ord(data[0]),ord(data[1]),ord(data[2]))
