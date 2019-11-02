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
            s = ser.readline()
        except SerialException as e:
            ser.close()
    return s

def write(msg):
    if ser.is_open:
        try:
            ser.write(msg)
        except SerialException as e:
            ser.close()

def connect(p, b):
    ser.close()
    ser.port = p
    ser.baudrate = b
    try:
        ser.open()
    except SerialException as e:
        pass
    write(b'<<?init-monitor?>>')

def isConnected():
    return ser.is_open
