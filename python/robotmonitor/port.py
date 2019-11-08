import robotmonitor.logging
from robotmonitor.logging import ErrorLevel

import serial
import serial.tools.list_ports
from serial.serialutil import SerialException


ser = serial.Serial()

def listPorts():
    ports = serial.tools.list_ports.comports()
    result = []
    for port in sorted(ports):
        result.append(port.device)
    return result

def read():
    s = ""
    if ser.is_open:
        try:
            s = ser.read(ser.in_waiting)
        except Exception as e:
            robotmonitor.logging.log(str(e), ErrorLevel.FATAL)
            ser.close()
    return s

def write(s):
    if ser.is_open:
        try:
            ser.write(s.encode())
        except SerialException as e:
            robotmonitor.logging.log(str(e), ErrorLevel.FATAL)
            ser.close()

def connect(p, b):
    ser.close()
    ser.port = p
    ser.baudrate = b
    try:
        ser.open()
    except SerialException as e:
        robotmonitor.logging.log(str(e), ErrorLevel.FATAL)

def isConnected():
    return ser.is_open

