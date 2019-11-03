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


class Commands(IntEnum):
    DEFAULT          = 0
    RESET            = 1
    PRINT            = 2
    SET              = 3
    ADD_WIDGET       = 4
    PLOT_INIT        = 5
    PLOT             = 6
    MODEL_INIT       = 7
    MODEL_COORDINATE = 8
    MODEL_ROTATION   = 9

class WidgetClasses(IntEnum):
    DEFAULT = 0
    BUTTON  = 1
    TEXTCTRL = 2
    SLIDER  = 3

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
            elif args[0] == "textctrl":
                cls = WidgetClasses.TEXTCTRL.value
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

