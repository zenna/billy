#!/usr/bin/env python
# Emily Mackevicius
# Nov 25, 2012

import wx
import wx.py
import sys,time,serial
from select import *



def quit():
   sys.exit()

"""def idle(parent):
   #
   # idle loop
   #
   wait = ser.inWaiting()
   global byte
   if (wait != 0):
      #
      # read character
      #
      byte = ser.read()
   #time.sleep(0.001)
   parent.after_idle(idle,parent)"""



class MyFrame(wx.Frame):
    def __init__(self,parent,title):
        wx.Frame.__init__(self,parent, title=title, size=(800,600))
        class Editor(wx.py.editwindow.EditWindow):
            def __init__(self,parent, margins=True, **kwargs):
                wx.py.editwindow.EditWindow.__init__(self,parent, **kwargs)
            @property
            def text(self):
                return self.GetText()
        global Ed, ser
        Ed = Editor(self, parent, size = (1,1))
        self.CreateStatusBar()
        # create the OK button
        OKbutton = wx.Button(self, wx.ID_OK, pos = (200,500), label = 'evaluate')
        self.Bind(wx.EVT_BUTTON, self.OnClick, OKbutton)
        # list controls
        ControlText = "Vehicle Controls: \nRfor(speed)\nRback(speed)\nLfor(speed)\nLback(speed)\n0<=speed<255\n\n\n\nsense0, sense1 = Sense()\n tilt = Tilt()"
        VehicleControls = wx.StaticText(self, -1, ControlText, pos = (600,100))
        # change to have GUI entries...
        port = "/Dev/tty.usbserial-FTFBGOT5"
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
        #make a menu
        filemenu=wx.Menu()
        # wx.ID_ABOUT and wx.ID_EXIT are standard IDs provided by wxWidgets.
        menuAbout = filemenu.Append(wx.ID_ABOUT, "&About"," Information about this program")
        filemenu.AppendSeparator()
        menuExit = filemenu.Append(wx.ID_EXIT,"&Exit"," Terminate the program")
        self.Bind(wx.EVT_MENU, self.OnAbout, menuAbout)
        self.Bind(wx.EVT_MENU, self.OnExit, menuExit)
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File") # Adding the "filemenu" to the MenuBar
        self.SetMenuBar(menuBar)  # Adding the MenuBar to the Frame content.
        self.Show(True)
        #self.Bind(wx.EVT_IDLE, self.OnIdle)
    def OnAbout(self,event):
            #dialog box
        dlg = wx.MessageDialog(self, "Press OK to execute code \n by Emily Mackevicius", "Vehicle brain", wx.OK)
        dlg.ShowModal()
        dlg.Destroy()
    def OnExit(self,event):
        self.Close(True)
    def OnClick(self,event):
        global Ed, ser
        def Sense():
           time.sleep(.01)
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
           time.sleep(.01)
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
        delay = 0 #for 115200 baud rate use .002
        def Rfor(a):
           for i in range(n_retries):
              #ser.flushInput()
              #ser.flushOutput()
              ser.write('m')
              ser.write(chr(a))
              print "r sending"
              time.sleep(delay)
        def Rback(a):
           for i in range(n_retries):
              #ser.flushInput()
              #ser.flushOutput()
              ser.write('n')
              ser.write(chr(a))
              print "l sending"
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
        code_globals = {'ser': ser, 'Sense': Sense, 'Rfor': Rfor, 'Rback': Rback, 'Lfor': Lfor, 'Lback':Lback, 'Tilt': Tilt}
        code_locals = {}
        code_object = compile("import sys,time,serial\nfrom select import *\n" + Ed.text, '<string>', 'exec')
        exec code_object in code_globals, code_locals
        #ser.write(code_globals['command'])
app = wx.App(False)  # Create a new app, don't redirect stdout/stderr to a window.
frame = MyFrame(None, "Emily's Editor")
app.MainLoop()


"""    def Motor(R_turn, L_turn):
           ser.flushInput()
           ser.flushOutput()
           if (R_turn>0):
              Rf = R_turn
              Rb = 0
           elif (R_turn<0):
              Rb = -R_turn
              Rf = 0
           else:
              Rb = 0
              Rf = 0
           if (L_turn>0):
              Lf = L_turn
              Lb = 0
           elif (L_turn<0):
              Lb = -L_turn
              Lf = 0
           else:
              Lb = 0
              Lf = 0
           ser.write('m')
           ser.write(chr(Rf))
           ser.write(chr(Rb))
           ser.write(chr(Lf))
           ser.write(chr(Lb))"""
