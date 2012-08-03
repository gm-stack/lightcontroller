#!/opt/local/bin/python2.7
import pypm,socket,time,random

devicename = pypm.GetDeviceInfo(1)[1]
print "Using device %s" % devicename

device = pypm.Input(1)

timecount = 0

def setLight(r,g,b):
	r_c = chr(int(r))
	g_c = chr(int(g))
	b_c = chr(int(b))
	return(r_c+g_c+b_c)

#colours = [(127,127,127),(255,0,0),(0,255,0),(0,0,255)]
colours = [(0,0,0)]

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while 1:
    msg = device.Read(1)
    msglen = len(msg)
    if msglen > 0:
        midicode =  msg[0][0][0]
        if midicode == 248:
            timecount += 1
            fracbeat = timecount % 24
            beat = int(timecount/24)
            if (fracbeat == 0):
                colours = [((random.randint(0,255)),(random.randint(0,200)),(random.randint(0,200)))]
            beatinbar = int(timecount/24)%1
            #print "%i:%i:%i" % (beat,beatinbar,fracbeat)
            colourmult = (23.0-fracbeat)/23.0
            r = colours[beatinbar][0]*colourmult
            g = colours[beatinbar][1]*colourmult
            b = colours[beatinbar][2]*colourmult
            sock.sendto(setLight(r,g,b),("127.0.0.1",54127))
            

            
