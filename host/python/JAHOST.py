#NVDAHEADER#
# Copyright (c) 2013 - present NVIDIA Corporation.  All rights reserved.
#
# NVIDIA Corporation and its licensors retain all intellectual property
# and proprietary rights in and to this software and related documentation
# and any modifications thereto.  Any use, reproduction, disclosure or
# distribution of this software and related documentation without an express
# license agreement from NVIDIA Corporation is strictly prohibited.
#NVDAHEADER#
import sys
import os
import ctypes
import sys, time
import struct
import serial,platform
from serial import *
from time import *
from serial.win32 import ULONG_PTR, is_64bit

def helpmessage():
	print('NvVserialManage v1.0.0')
	print('')
	print('NvVserialManage [Ports]')
	print('Ports: there are n Ports of Virtual Serial Port will be created')

class JAHost(object):
  def __init__(self):
    self.serial = serial.Serial()
    
  def start(self):
    self.serial.open()
    


if __name__=='__main__':
  # optionally pass device/directory names to watch for notifications.
  # Eg, plug in a USB device - assume it connects as E: - then execute:
  # % win32gui_devicenotify.py E:
  # Then remove and insert the device.
  ser = serial.Serial(
    port='/dev/ttyUSB1',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
  )
  print("jtest")
  #print(sys.argv)
  if len(sys.argv) >=2:
    if(sys.argv[1] == "-help" or sys.argv[1] == "-?"):
      helpmessage()
      sys.exit("error")



