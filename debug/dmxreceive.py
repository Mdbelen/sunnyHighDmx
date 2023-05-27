
import serial;


def printrgb(data):
    print("r:%3d g:%3d b:%3d" % (data[0], data[1], data[2]))

def evaldmx(data):
    par00 = data[0:6]
    printrgb(par00[0:3])


ser = serial.Serial("COM5", 250000, 8, "N", 2, 0.1)

dmxstate = 0

while 1:
    dmxstream = ser.read(500)

    if len(dmxstream) > 100:
        #print(len(dmxstream))
        for i in range(80, len(dmxstream)-3):
            if dmxstream[i] == 127 and dmxstream[i+1] == 127 and dmxstream[i+2] == 255:
                dmxpack = dmxstream[i-79:i+1]
                evaldmx(dmxpack)


