#!/opt/local/bin/python2.7
import pypm,socket,time

devicename = pypm.GetDeviceInfo(1)[1]
print "Using device %s" % devicename

device = pypm.Input(1)

timecount = 0

colours = ["\x7F\x7F\x7F","\xFF\x00\x00","\x00\xFF\x00","\x00\x00\xFF"]

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while 1:
    msg = device.Read(1)
    msglen = len(msg)
    if msglen > 0:
        midicode =  msg[0][0][0]
        if midicode == 248:
            timecount += 1
            beat = int(timecount/24)
            beatinbar = int(timecount/24)%4
            print "%i:%i" % (beat,beatinbar)
            sock.sendto(colours[beatinbar],("172.22.0.55",54127))
            

            