from time import sleep
from calendar import c
from tkinter import *
from turtle import width
import serial.tools.list_ports
import serial
import struct
import time
import datetime
from threading import Thread
import binascii
import copy
import json

class EMBoardTool:
    def __init__(self):
        # root window title and dimension
        self.root = Tk() 
        self.root.title("GRSS Tool") 
        self.root.geometry('700x700')
        menu = Menu(self.root) 
        item = Menu(menu) 
        item.add_command(label='New') 
        menu.add_cascade(label='File', menu=item) 
        self.root.config(menu=menu)

        #define variable here
        self.mSerialComm = 0
        self.mSerialPort = '' 
        self.ImageBuffer = []
        self.DataImage   = 1

        self.mDurationStart = datetime.datetime.now()
        self.mDurationFinish = datetime.datetime.now()
        self.mDurationDelta = datetime.datetime.now()

        #define Command Here
        self.LIST_OF_COMMAND = {
            #NOR Command
            'mCommand_NOR_CHECK_NOR_HARDWARE_CONNECTION'        : [125, 1, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_NOR_READ_ALL_IMAGE_STATUS'                : [125, 1, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_NOR_READ_SINGLE_IMAGE'                    : [125, 1, 3,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_NOR_READ_MULTIPLE_IMAGE'                  : [125, 1, 4,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_NOR_TOTAL_IMAGE'                          : [125, 1, 6,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_NOR_ERASE_ALL_SECTOR'                     : [125, 1, 8,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_NOR_ERASE_SINGLE_IMAGE'                   : [125, 1, 9,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_NOR_ERASE_MULTIPLE_IMAGE'                 : [125, 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            'mCommand_NOR_RELOAD_CONFIGURATION'                 : [125, 1, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_NOR_READ_ALL_CONFIGURATION'               : [125, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK 
            'mCommand_NOR_SET_ALL_CONFIG_TO_DEFAULT'            : [125, 1, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_NOR_ERASE_SINGLE_IMAGE_LOW_RES'           : [125, 1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            'mCommand_NOR_ERASE_SINGLE_IMAGE_HIGH_RES'          : [125, 1, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            'mCommand_NOR_ERASE_MULTIPLE_IMAGE_LOW_RES'         : [125, 1, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            'mCommand_NOR_ERASE_MULTIPLE_IMAGE_HIGH_RES'        : [125, 1, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],

            #Camera Command
            'mCommand_CAMERA_REINIT'                            : [125, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_CAMERA_TAKE_SINGLE_IMAGE'                 : [125, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_CAMERA_TAKE_MULTIPLE_IMAGE'               : [125, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], #SERIAL OK
            'mCommand_UPDATE_SETTING_BRIGHTNESS'                : [125, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_CONTRAST'                  : [125, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_LIGHT'                     : [125, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_SATURATION'                : [125, 2, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_HUE'                       : [125, 2, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_SPECIAL'                   : [125, 2, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_EXPOSURE'                  : [125, 2, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_SHARPNESS'                 : [125, 2, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_MIRROR'                    : [125, 2, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_COMPRESS_QUALITY'          : [125, 2, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_SETTING_RESOLUTION'                : [125, 2, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_UPDATE_ALL_CONFIG_TO_CAMERA'              : [125, 2, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_CAMERA_TAKE_BY_TIME'                      : [125, 2, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 

            #Temperature Command
            'mCommand_TEMP_READ_SINGLE_TEMP'                    : [125, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_TEMP_READ_TEMP_SCHEDULER'                 : [125, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_TEMP_READ_TEMP_TIMER'                     : [125, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_TEMP_READ_TEMP_SECTOR'                    : [125, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_TEMP_UPDATE_TEMP_TIMER'                   : [125, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 
            'mCommand_TEMP_UPDATE_TEMP_SCHEDULER'               : [125, 3, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 

            #MUX Command
            'mCommand_MUX_REQUEST_NOR_ACCESS_COMMAND'           : [125, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        }

        self.OPTIONS = [] 
        for command in self.LIST_OF_COMMAND:
            self.OPTIONS.append(command)
        self.variable = StringVar(self.root)
        self.variable.set(self.OPTIONS[0])
        
        #Execute Pre start Apps
        self.BoxPort = self.createTextBoxPort()
        self.BaudratePort = self.createEntryBoxBaudrate()
        self.COMPort = self.createEntryBoxPortToOpen()
        self.COMPortState = self.createTextBoxPortState()
        self.Payload1Box = self.createBoxPayload1()
        self.Payload2Box = self.createBoxPayload2()
        self.TerminalBox = self.createTerminalBox()
        self.iNum = 1
    
    def serialize_U32(self, iValue):
        if iValue < 0:
            return False
        else:
            mAddress = [0,0,0,0]
            mAddress[0] = (iValue >> 24) & 0xFF
            mAddress[1] = (iValue >> 16) & 0xFF
            mAddress[2] = (iValue >>  8) & 0xFF
            mAddress[3] = (iValue >>  0) & 0xFF
        return mAddress
    
    def DeserializeU32(self, iValueList):
        retVal = 0
        retVal |= iValueList[0] 
        retVal <<= 8
        retVal |= iValueList[1] 
        retVal <<= 8
        retVal |= iValueList[2] 
        retVal <<= 8
        retVal |= iValueList[3]
        return retVal

    def createMenuList(self):
        drop = OptionMenu( self.root, self.variable, *self.OPTIONS)
        drop.pack() 
        drop.place(x = 0, y = 90)
        drop.config(width=25)

    def createTextBoxPort(self):
        mBoxPort = Text(self.root, width=10, height=5) 
        mBoxPort.place(x=100, y=0)
        return mBoxPort
    
    def createTextBoxPortState(self):
        mSerialPortState = Text(self.root, width=10,  height=1)
        mSerialPortState.place(x =400, y = 0)
        return mSerialPortState
        
    def createTerminalBox(self):
        mTerminalBox = Text(self.root, width=70, height=30)
        mTerminalBox.place(x = 5, y = 160)
        return mTerminalBox
    
    def btnScanclicked(self): 
        self.mSerialPort = serial.tools.list_ports.comports()
        self.mSerialPort = [port.device for port in self.mSerialPort]
        self.mSerialPort = ("\n".join(map(str, self.mSerialPort)))
        self.BoxPort.insert(END, self.mSerialPort)
    
    def btnClearclicked(self):
    	self.BoxPort.delete('1.0', END)
        
    def createBoxTotalImage(self):
        mBoxTotalImage = Text(self.root, width=10, height=1) 
        mBoxTotalImage.place(x=110, y=120)
        return mBoxTotalImage    

    def createTakeSingleImage(self):
        btnReadTotalSavedImage = Button(self.root, text= "Take Single Image",
                    fg= "black", command=self.btnTakeSingleImageClicked)
        btnReadTotalSavedImage.place(x = 0, y = 150)
    
    def createLabelBuffState(self):
        lblBaudrate = Label(self.root, text = "Image Buff State")  
        lblBaudrate.place (x =0, y = 150)
    
    def createButtonGetImageBuffer(self):
        btnTakePicnotSaving = Button(self.root, text = "Get Image Buffer" , 
                    fg = "black", command=self.btnGetImageBuffer) 
        btnTakePicnotSaving.place(x=0, y=180)
    
    def createTextBoxBuffImageState(self):
        mBuffImageState = Text(self.root, width=10,  height=1)
        mBuffImageState.place(x = 50, y = 150)
        return mBuffImageState
    
    def createButtonScanPort(self):
        btnScan = Button(self.root, text = "Scan Serial Port" , 
        			fg = "black", command=self.btnScanclicked) 
        btnScan.grid(column=0, row=0)
    
    def createButtonClearPort(self):
        btnClearPort = Button(self.root, text = "Clear Port List" , 
                    fg = "black", command=self.btnClearclicked) 
        btnClearPort.place(x=0, y=30)
    
    def createButtonOpenPort(self):
        btnOpen = Button(self.root, text = "Open Serial Port" , 
                    fg = "black", command=self.btnOpenclicked) 
        btnOpen.place(x=190, y=55)

    def createButtonSendCommand(self):
        btnSendCommand = Button(self.root, text = "Send Command" , 
                    fg = "black", command=self.btnSendCommandClicked) 
        btnSendCommand.place(x=350, y=90)

    def createButtonClearTerminal(self):
        btnSendCommand = Button(self.root, text = "Clear Terminal" , 
                    fg = "black", command=self.btnClearTerminalClicked) 
        btnSendCommand.place(x=350, y=120)        
    
    def createButtonSendContinous(self):
        btnSendCon = Button(self.root, text = "Send Continous",
                fg = "black", command=self.btnSendConClicked)
        btnSendCon.place(x = 470, y = 120)
    
    def btnSendConClicked(self):
        mDuration  = int(self.DurationBox.get())
        mIteration = 0

        for i in range(mIteration):
            self.btnSendCommandClicked()
            self.iNum += 1
            sleep(mDuration)

    def ConvertDatatoImage(self, iData):
        isHeader = False
        isFooter = False
        if iData[0] == 255 and iData[1] == 216:
            isHeader = True
        if(isHeader == True):
            mImage = []
            if isHeader == True:
                for i in range(len(iData)):
                    if(i > 1):
                        mImage.append(iData[i])
                        if(iData[i] == 217 and iData[(i-1)] == 255):
                            isFooter = True
                            break
                    else:
                        mImage.append(iData[i])
            if isHeader == True and isFooter == True:
                b = ""
                data = ""                
                with open('C:/Users/ASUS/Videos/GUI/GUI_19102022/GAMBAR/DATAIMAGE.txt', 'a') as fileops:
                    json.dump(mImage, fileops)                                    
                for x in mImage:
                    a = str(hex(x))
                    b = a.replace('0x', '')
                    if(len(b) == 1):
                        c = '0' + b
                    else:
                        c = b
                    data += c
                    data += " "
                data = data.upper()
                data=data.strip()
                data=data.replace(' ', '')
                data=data.replace('\n', '')
                data = binascii.a2b_hex(data)

                with open('C:/Users/ASUS/Videos/GUI/GUI_19102022/GAMBAR/Image'+str(self.DataImage)+'.jpg', 'wb') as image_file:
                    image_file.write(data)
                self.DataImage += 1    
                return True
        else:
            return False

    def BuildCommand(self, iPayload1, iPayload2, iCommand):
        mArrayPayload1 = self.serialize_U32(iPayload1)
        mArrayPayload2 = self.serialize_U32(iPayload2)
        if mArrayPayload1 != False and mArrayPayload2 != False:
            mCommand = copy.deepcopy(self.LIST_OF_COMMAND[iCommand])
            mCommand[3] = mArrayPayload1[0]
            mCommand[4] = mArrayPayload1[1]
            mCommand[5] = mArrayPayload1[2]
            mCommand[6] = mArrayPayload1[3]

            mCommand[7]  = mArrayPayload2[0]
            mCommand[8]  = mArrayPayload2[1]
            mCommand[9]  = mArrayPayload2[2]
            mCommand[10] = mArrayPayload2[3]
            return mCommand     
        else:
            return False

    def btnClearTerminalClicked(self):
        self.TerminalBox.delete('1.0', END)
    
    def btnSendCommandClicked(self):
        mPayload1       = self.Payload1Box.get()
        mPayload2       = self.Payload2Box.get()
        mCommand        = self.variable.get()
        mCommandList    = []
        if(mPayload1 == '' or mPayload2 == ''):
            self.TerminalBox.insert(END,"PAYLOAD CAN NOT EMPTY \n")
            return
        else:
            self.TerminalBox.insert(END,"Command: ")
            self.TerminalBox.insert(END,mCommand )
            self.TerminalBox.insert(END, "\n")
            mCommandList    = self.BuildCommand(int(mPayload1), int(mPayload2), mCommand)
        mStringCommand = b""
        for mByte in range(len(mCommandList)):
            a = struct.pack("B", mCommandList[mByte])
            mStringCommand += a
        self.mSerialComm.flushOutput()
        self.mSerialComm.flushInput()
        self.mDurationStart = datetime.datetime.now()
        self.mSerialComm.write(mStringCommand)  


        if(mCommand == "mCommand_NOR_READ_ALL_IMAGE_STATUS"):
            isWaitingComplete = True
            mBufferSize = 40
            data = []
            mList = []
            while isWaitingComplete == True:
                data = list(bytearray(self.mSerialComm.read(mBufferSize)))
                mList.extend(data)
                data = []
                if(len(mList) == mBufferSize):
                    self.TerminalBox.insert(END, "Data: ")
                    self.TerminalBox.insert(END, mList)
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "ACK: ")
                    mACKData = list(bytearray(self.mSerialComm.read(13)))
                    self.TerminalBox.insert(END, mACKData)
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")
                    isWaitingComplete = False

        elif(mCommand == "mCommand_NOR_READ_SINGLE_IMAGE"):
            if int(mPayload1) == 0:
                mBufferSize = 262140
                isWaitingComplete = True
                mDataList = []
                while isWaitingComplete == True:
                    if(len(mDataList) == 262016):
                        mImagedata = list(bytearray(self.mSerialComm.read(124)))
                    else:
                        mImagedata = list(bytearray(self.mSerialComm.read(128)))
                    mDataList.extend(mImagedata)
                    mImagedata = []
                    if(len(mDataList) == mBufferSize): #Size for One Image
                        if self.ConvertDatatoImage(mDataList) == True:
                            self.TerminalBox.insert(END, "DATA: ")
                            self.TerminalBox.insert(END, "IMAGE GENERATED")
                            self.TerminalBox.insert(END, "\n")
                            self.TerminalBox.insert(END, "\n")
                        else:
                            self.TerminalBox.insert(END, "DATA: ")
                            self.TerminalBox.insert(END, "IMAGE CORRUPTED")
                            self.TerminalBox.insert(END, "\n")
                            self.TerminalBox.insert(END, "\n")
                        self.TerminalBox.insert(END, "ACK: ")
                        mACKData = list(bytearray(self.mSerialComm.read(13)))
                        isWaitingComplete = False
                        self.TerminalBox.insert(END, mACKData)
                        self.mDurationFinish = datetime.datetime.now()
                        self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                        self.TerminalBox.insert(END, "\n")
                        self.TerminalBox.insert(END, "Execution Time: ")
                        self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                        self.TerminalBox.insert(END, "\n")
                        self.TerminalBox.insert(END, "\n")
            elif int(mPayload1) == 1:
                isWaitingComplete = True
                mDataList = []
                mBufferSize = 1310700
                while isWaitingComplete == True:
                    if(len(mDataList) == 1310592):
                        mImagedata = list(bytearray(self.mSerialComm.read(108)))
                    else:
                        mImagedata = list(bytearray(self.mSerialComm.read(128)))
                    mDataList.extend(mImagedata)
                    mImagedata = []
                    if(len(mDataList) == mBufferSize): #Size for One Image
                        if self.ConvertDatatoImage(mDataList) == True:
                            self.TerminalBox.insert(END, "DATA: ")
                            self.TerminalBox.insert(END, "IMAGE GENERATED")
                            self.TerminalBox.insert(END, "\n")
                            self.TerminalBox.insert(END, "\n")
                        else:
                            self.TerminalBox.insert(END, "DATA: ")
                            self.TerminalBox.insert(END, "IMAGE CORRUPTED")
                            self.TerminalBox.insert(END, "\n")
                            self.TerminalBox.insert(END, "\n")
                        self.TerminalBox.insert(END, "ACK: ")
                        mACKData = list(bytearray(self.mSerialComm.read(13)))
                        isWaitingComplete = False
                        self.TerminalBox.insert(END, mACKData)
                        self.mDurationFinish = datetime.datetime.now()
                        self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                        self.TerminalBox.insert(END, "\n")
                        self.TerminalBox.insert(END, "Execution Time: ")
                        self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                        self.TerminalBox.insert(END, "\n")
                        self.TerminalBox.insert(END, "\n")

        elif(mCommand == "mCommand_NOR_READ_MULTIPLE_IMAGE"):
            if int(mPayload1) == 0:
                mBufferSize = 262140
                mDataImage = []
                for i in range(int(mPayload2)):
                    mDataList = []
                    isWaitingComplete = True
                    while isWaitingComplete == True:
                        if(len(mDataList) == 262016):
                            mImagedata = list(bytearray(self.mSerialComm.read(124)))
                        else:
                            mImagedata = list(bytearray(self.mSerialComm.read(128)))
                        mDataList.extend(mImagedata)
                        if(len(mDataList) == mBufferSize): 
                            mDataImage.append(mDataList)
                            mImagedata = list(bytearray(self.mSerialComm.read(4)))
                            isWaitingComplete = False
                time.sleep(0.2)
                mACKData = list(bytearray(self.mSerialComm.read(13)))

                for i in range(len(mDataImage)):
                    if self.ConvertDatatoImage(mDataImage[i]) == True:
                        print("GENERATED IMAGE")
                        self.TerminalBox.insert(END, "DATA: ")
                        self.TerminalBox.insert(END, "IMAGE GENERATED")
                        self.TerminalBox.insert(END, "\n")
                    else:
                        print("ERROR GENERATED")

                print("ACK", mACKData)
                self.TerminalBox.insert(END, "ACK: ")
                self.TerminalBox.insert(END, mACKData)
                self.mDurationFinish = datetime.datetime.now()
                self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                self.TerminalBox.insert(END, "\n")
                self.TerminalBox.insert(END, "Execution Time: ")
                self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                self.TerminalBox.insert(END, "\n")
                self.TerminalBox.insert(END, "\n")
            elif int(mPayload1) == 1:
                mBufferSize = 1310700
                mDataImage = []
                for i in range(int(mPayload2)):
                    mDataList = []
                    isWaitingComplete = True
                    while isWaitingComplete == True:
                        if(len(mDataList) == 1310592):
                            mImagedata = list(bytearray(self.mSerialComm.read(108)))
                        else:
                            mImagedata = list(bytearray(self.mSerialComm.read(128)))
                        mDataList.extend(mImagedata)
                        if(len(mDataList) == (mBufferSize)): 
                            mDataImage.append(mDataList)                            
                            mImagedata = list(bytearray(self.mSerialComm.read(4)))
                            isWaitingComplete = False

                self.TerminalBox.insert(END, "ACK: ")
                self.TerminalBox.insert(END, "\n")
                mACKData = list(bytearray(self.mSerialComm.read(13)))

                for i in range(len(mDataImage)):
                    if self.ConvertDatatoImage(mDataImage[i]) == True:
                        print("GENERATED IMAGE")
                        self.TerminalBox.insert(END, "DATA: ")
                        self.TerminalBox.insert(END, "IMAGE GENERATED")
                        self.TerminalBox.insert(END, "\n")
                    else:
                        print("ERROR GENERATED")

                self.TerminalBox.insert(END, mACKData)
                self.mDurationFinish = datetime.datetime.now()
                self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                self.TerminalBox.insert(END, "\n")
                self.TerminalBox.insert(END, "Execution Time: ")
                self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                self.TerminalBox.insert(END, "\n")
                self.TerminalBox.insert(END, "\n")
            
        elif(mCommand == "mCommand_NOR_READ_ALL_CONFIGURATION"):
            isWaitingComplete = True
            data = []
            mList = []
            while isWaitingComplete == True:
                data = list(bytearray(self.mSerialComm.read(200)))
                mList.extend(data)
                data = []
                if(len(mList) == 200):                    
                    mConfigData = []
                    mConfigDataList = []
                    for i in range(len(mList)):
                        mConfigData.append(mList[i])
                        if(len(mConfigData) == 4):
                            mConfigDataList.append(self.DeserializeU32(mConfigData))
                            mConfigData = []
                    print(mConfigDataList)
                    self.TerminalBox.insert(END, "CAM_LOW_RESOLUTION:")
                    self.TerminalBox.insert(END, mConfigDataList[0])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_HIGH_RESOLUTION:")
                    self.TerminalBox.insert(END, mConfigDataList[1])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_QUALITY: ")
                    self.TerminalBox.insert(END, mConfigDataList[2])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_BRIGHTNESS: ")
                    self.TerminalBox.insert(END, mConfigDataList[3])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_CONTRAST: ")
                    self.TerminalBox.insert(END, mConfigDataList[4])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "TEMPERATURE_TIMER: ")
                    self.TerminalBox.insert(END, mConfigDataList[5])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "TEMPERATURE_SCHEDULER: ")
                    self.TerminalBox.insert(END, mConfigDataList[6])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_LIGHT: ")
                    self.TerminalBox.insert(END, mConfigDataList[7])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_HUE: ")
                    self.TerminalBox.insert(END, mConfigDataList[8])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_SATURATION: ")
                    self.TerminalBox.insert(END, mConfigDataList[9])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_SPECIAL: ")
                    self.TerminalBox.insert(END, mConfigDataList[10])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_EXPOSURE: ")
                    self.TerminalBox.insert(END, mConfigDataList[11])
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "CAM_SHARPNESS: ")
                    self.TerminalBox.insert(END, mConfigDataList[12])
                    self.TerminalBox.insert(END, "\n")

                    self.TerminalBox.insert(END, "ACK: ")
                    mACKData = list(bytearray(self.mSerialComm.read(13)))
                    self.TerminalBox.insert(END, mACKData)
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")
                    isWaitingComplete = False
        
        elif(mCommand == 'mCommand_TEMP_START_CONTINUOS_TEMP'):
            isWaitingData = 1
            mData = []
            while isWaitingData == 1:
                mData = list(bytearray(self.mSerialComm.read(13)))
                if(len(mData) == 13):
                    isWaitingData = 0
                    self.TerminalBox.insert(END, "ACK: ")
                    self.TerminalBox.insert(END, mData)
                    self.TerminalBox.insert(END, "\n")
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")

        elif(mCommand == 'mCommand_TEMP_STOP_CONTINUOS_TEMP'):  
            isWaitingData = 1
            mData = []
            while isWaitingData == 1:
                mData = list(bytearray(self.mSerialComm.read(13)))
                if(len(mData) == 13):
                    isWaitingData = 0
                    self.TerminalBox.insert(END, "ACK: ")
                    self.TerminalBox.insert(END, mData)
                    self.TerminalBox.insert(END, "\n")
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")

        elif(mCommand == 'mCommand_TEMP_READ_SINGLE_TEMP'):     
            isWaitingComplete = True
            mData = []
            mList = []
            while isWaitingComplete == True:
                mData = list(bytearray(self.mSerialComm.read(4)))
                mList.extend(mData)
                if(len(mList) == 4):
                    self.TerminalBox.insert(END, "Data: ")
                    mTemperature = self.DeserializeU32(mList)
                    self.TerminalBox.insert(END, mTemperature)
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "ACK: ")
                    mACKData = list(bytearray(self.mSerialComm.read(13)))
                    self.TerminalBox.insert(END, mACKData)
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")
                    isWaitingComplete = False

        elif(mCommand == 'mCommand_TEMP_READ_TEMP_SCHEDULER'):
            isWaitingData = 1
            mData = []
            while isWaitingData == 1:
                mData = list(bytearray(self.mSerialComm.read(13)))
                if(len(mData) == 13):
                    isWaitingData = 0
                    self.TerminalBox.insert(END, "ACK: ")
                    self.TerminalBox.insert(END, mData)
                    self.TerminalBox.insert(END, "\n")
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")

        elif(mCommand == 'mCommand_TEMP_READ_TEMP_TIMER'):    
            isWaitingComplete = True
            mData = []
            mList = []
            while isWaitingComplete == True:
                mData = list(bytearray(self.mSerialComm.read(4)))
                mList.extend(mData)
                if(len(mList) == 4):
                    self.TerminalBox.insert(END, "Data: ")
                    mTimer = self.DeserializeU32(mList)
                    self.TerminalBox.insert(END, mTimer)
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "ACK: ")
                    mACKData = list(bytearray(self.mSerialComm.read(13)))
                    self.TerminalBox.insert(END, mACKData)
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")
                    isWaitingComplete = False

        elif(mCommand == 'mCommand_TEMP_READ_TEMP_SECTOR'):   
            isWaitingComplete = True
            mData = []
            mList = []
            while isWaitingComplete == True:
                mData = list(bytearray(self.mSerialComm.read(800)))
                mList.extend(mData)
                if(len(mList) == 800):
                    mTemperatureData = []
                    mTemperatureDataList = []
                    for i in range(800):
                        mTemperatureData.append(mList[i])
                        if(len(mTemperatureData) == 4):                            
                            mOneData = self.DeserializeU32(mTemperatureData)
                            mTemperatureDataList.append(mOneData)
                            mTemperatureData = []

                    self.TerminalBox.insert(END, "Data: ")
                    self.TerminalBox.insert(END, mTemperatureDataList)
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "ACK: ")
                    mACKData = list(bytearray(self.mSerialComm.read(13)))
                    self.TerminalBox.insert(END, mACKData)
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")
                    isWaitingComplete = False

        else:            
            wait = 1
            data = []
            while wait == 1:
                data = list(bytearray(self.mSerialComm.read(13)))
                if(len(data) == 13):
                    wait = 0
                    self.TerminalBox.insert(END, "ACK: ")
                    self.TerminalBox.insert(END, data)
                    self.TerminalBox.insert(END, "\n")
                    self.mDurationFinish = datetime.datetime.now()
                    self.mDurationDelta = self.mDurationFinish - self.mDurationStart
                    self.TerminalBox.insert(END, "Execution Time: ")
                    self.TerminalBox.insert(END, self.mDurationDelta.total_seconds())
                    self.TerminalBox.insert(END, "\n")
                    self.TerminalBox.insert(END, "\n")

    def btnOpenclicked(self):
        baudrate = self.BaudratePort.get()
        port =  self.COMPort.get()
        try:
            # self.mSerialComm = serial.Serial(port, baudrate, timeout=4)
            self.mSerialComm = serial.Serial(port, baudrate)
            self.COMPortState.delete('1.0', END)
            self.COMPortState.insert(END, "OPENED")
        except:
            self.COMPortState.delete('1.0', END)
            self.COMPortState.insert(END, "ERROR")

    def createButtonClosePort(self):
        btnOpen = Button(self.root, text = "Close Serial Port" , 
                    fg = "black", command=self.btnCloseclicked) 
        btnOpen.place(x=300, y=55)
    
    def btnCloseclicked(self):
        try:
            self.mSerialComm.close()
            self.COMPortState.delete('1.0', END)
            self.COMPortState.insert(END, "CLOSED")
        except:
            self.COMPortState.delete('1.0', END)
            self.COMPortState.insert(END, "ERROR")
    
    def createlabelPorttoOpen(self):
        lblPort = Label(self.root, text = "Port")  
        lblPort.place (x=190, y=0)
    
    def createLabelBaudrate(self):
        lblBaudrate = Label(self.root, text = "Baudrate")  
        lblBaudrate.place (x=190, y=30)
    
    def createLabelSerialState(self):
        lblBaudrate = Label(self.root, text = "Port State")  
        lblBaudrate.place (x =330, y = 0)
    
    def createLabelTestState(self):
        lblBaudrate = Label(self.root, text = "Test State")  
        lblBaudrate.place (x =300, y = 120)

    def createLabelPayload1(self):
        lblPayload1 = Label(self.root, text = "Payload1")
        lblPayload1.place(x = 210, y = 90)
    
    def createBoxPayload1(self):
        mBoxPayload1 = Entry(self.root, width=10) 
        mBoxPayload1.place(x=275, y=90)
        return mBoxPayload1  

    def createlabelPayload2(self):
        lblPayload2 = Label(self.root, text = "Payload2")
        lblPayload2.place(x = 210, y = 120)

    def createBoxPayload2(self):
        mBoxPayload2 = Entry(self.root, width=10) 
        mBoxPayload2.place(x=275, y=120)
        return mBoxPayload2  

    def createlabelDuration(self):
        lblDuration = Label(self.root, text = "Duration")
        lblDuration.place(x = 350, y = 120)
    
    def createBoxDuration(self):
        mBoxDuration = Entry(self.root, width=10) 
        mBoxDuration.place(x=400, y=120)
        return mBoxDuration 

    def createEntryBoxPortToOpen(self):
        mEntryPort = Entry(self.root, width=10) 
        mEntryPort.place(x=250, y=0)
        return mEntryPort

    def createEntryBoxBaudrate(self):
        mEntryBaudrate = Entry(self.root, width=10) 
        mEntryBaudrate.place(x=250, y=30)
        return mEntryBaudrate
    
    def mMainService(self):
        self.createMenuList()
        self.createButtonScanPort()
        self.createButtonClearPort()
        self.createButtonOpenPort()
        self.createLabelBaudrate()
        self.createlabelPorttoOpen()
        self.createLabelSerialState()
        self.createLabelPayload1()
        self.createlabelPayload2()
        self.createButtonSendCommand()
        self.createButtonClearTerminal()
        self.createButtonClosePort()
        
        #Mainloop Here !!!
        self.root.mainloop()

tool = EMBoardTool()
tool.mMainService()



