import MonitorPort
from enum import IntEnum


def splitCommandArgs(s):
    tokens = []
    token = ""
    isString = False
    for c in s:
        if isString:
            if c == '"':
                tokens.append(token)
                token = ""
                isString = False
            else:
                token = token + c
        else:
            if c == " ":
                if token:
                    tokens.append(token)
                    token = ""
            elif c == '"':
                isString = True
            else:
                token = token + c
    if token:
        tokens.append(token)
    return tokens


def sendCommand(cmd):
    MonitorPort.write(cmd)

def sendClickCommand(name):
    MonitorPort.write("<<?call %s?>>" % name)

def updateTextBox(name, value):
    MonitorPort.write("<<?set %s %s?>>" % (name, value))


class Commands(IntEnum):
    DEFAULT          = 0
    PRINT            = 1
    SET              = 2
    ADD_WIDGET       = 3
    PLOT_INIT        = 4
    PLOT             = 5
    MODEL_INIT       = 6
    MODEL_COORDINATE = 7
    MODEL_ROTATION   = 8

class WidgetClasses(IntEnum):
    BUTTON  = 0
    TEXTBOX = 1
    SLIDER  = 2

class InputType(IntEnum):
    CHAR   = 0
    INT    = 1
    FLOAT  = 2
    STRING = 3


cmdLine = ""
cmdQueue = []

def processCommand(s):
    ret = (Commands.DEFAULT.value, None)
    if not s:
        return ret
    tokens = splitCommandArgs(s)
    cmd = tokens[0]
    args = tokens[1:]
    if cmd == "set":
        if len(args) == 2:
            ret = (Commands.SET.value, args[0], args[1])
        else:
            print("Usage: set <name> <value>")
    elif cmd == "add-widget":
        if len(args) >= 3:
            cls = -1
            if args[0] == "button":
                cls = WidgetClasses.BUTTON.value
                ret = (Commands.ADD_WIDGET.value,
                       cls, args[1], args[2])
            elif args[0] == "textbox":
                cls = WidgetClasses.TEXTBOX.value
                t = -1
                if args[3] == "int":
                    t = InputType.INT.value
                elif args[3] == "float":
                    t = InputType.FLOAT.value
                elif args[3] == "string":
                    t = InputType.STRING.value
                
                if t != -1:
                    ret = (Commands.ADD_WIDGET.value,
                           cls, args[1], args[2], t)
            elif args[0] == "slider":
                cls = WidgetClasses.SLIDER.value
                ret = (Commands.ADD_WIDGET.value,
                       cls, args[1], args[2])
            else:
                print("No such widget %s exists" % args[0])
        else:
            print("Usage: add-widget <class> <name> <display text> <options>")
    else:
        print("Command not found")
    return ret


def getCommand():
    global cmdLine, cmdQueue, isCmd
    
    msg = MonitorPort.read()
    
    if not msg:
        return (Commands.DEFAULT.value, None)
    
    while True:
        i = msg.find("<<?")
        j = msg.find("?>>")
        if j != -1 and j < i:
            i = -1
        
        if i != -1 and j != -1:
            cmdQueue.append(msg[i+3:j])
            msg = msg[:i] + msg[j+3:]
        elif i != -1:
            cmdLine = msg[i+3:]
            msg = msg[:i]
            break
        elif j != -1:
            cmdLine += msg[:j]
            cmdQueue.append(cmdLine)
            line = ""
            msg = msg[j+3:]
        else:
            break
    
    if len(cmdQueue) > 0:
        return processCommand(cmdQueue.pop(0))
    elif msg:
        ret = (Commands.PRINT.value, msg[0:])
        msg = ""
        return ret

