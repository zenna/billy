import sys,time,serial
port = "/dev/serial/by-id/usb-FTDI_TTL232R_FTFBGOT5-if00-port0"
#PortText = wx.TextCtrl(self, -1, pos = (600, 50), size = (200,25))
#PortText.WriteText(port)
speed = 9600
#PortText = wx.TextCtrl(self, -1, pos = (600, 75), size = (200,25))
#PortText.WriteText(speed)
#
# open serial port
#
ser = serial.Serial(port, speed)
ser.setDTR()
ser.timeout = .1
#
# flush buffers
#
ser.flushInput()
ser.flushOutput()

def Sense():
   byte2 = 0
   byte3 = 0
   byte4 = 0
   ser.flushInput()
   ser.flushOutput()
   #print(ser.read())
   ser.write('s')
   while 1:
      byte1 = byte2
      byte2 = byte3
      byte3 = byte4
      byte4 = ord(ser.read())-ord('0')
      if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)):
         break
   a = ord(ser.read())
   b = ord(ser.read())
   sense0 = 256*b+a
   a = ord(ser.read())
   b = ord(ser.read())
   sense1 = 256*b+a
   return (sense0, sense1)
def Tilt():
   byte2 = 0
   byte3 = 0
   byte4 = 0
   ser.flushInput()
   ser.flushOutput()
   #print(ser.read())
   ser.write('t')
   while 1:
      byte1 = byte2
      byte2 = byte3
      byte3 = byte4
      byte4 = ord(ser.read())-ord('0')
      if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)):
         break
   a = ord(ser.read())
   b = ord(ser.read())
   sense0 = 256*b+a
   a = ord(ser.read())
   b = ord(ser.read())
   sense1 = 256*b+a
   return (sense0)
   #time.sleep(0.01)
n_retries = 1
delay = .002
def Rfor(a):
   for i in range(n_retries):
      #ser.flushInput()
      #ser.flushOutput()
      ser.write('m')
      ser.write(chr(a))
      time.sleep(delay)
def Rback(a):
   for i in range(n_retries):
      #ser.flushInput()
      #ser.flushOutput()
      ser.write('n')
      ser.write(chr(a))
      time.sleep(delay)
def Lfor(a):
   for i in range(n_retries):
      #ser.flushInput()
      #ser.flushOutput()
      ser.write('o')
      ser.write(chr(a))
      time.sleep(delay)
def Lback(a):
   for i in range(n_retries):
      #ser.flushInput()
      #ser.flushOutput()
      ser.write('p')
      ser.write(chr(a))
      time.sleep(delay)