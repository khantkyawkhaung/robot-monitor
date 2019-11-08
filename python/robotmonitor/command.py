import robotmonitor.port
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
    return ret


cmdKeys = ["<<?", "?>>"]
cmdDetectedIndecies = []
cmdDetectedWord = ""
isCmd = False
cmdLine = ""
cmdQueue = []

def getCommand():
    global cmdKeys, cmdDetectedIndecies, cmdDetectedWord
    global isCmd, cmdLine, cmdQueue
    
    msg = robotmonitor.port.read()
    toPrint = ""
    
    if msg:
        for c in msg:
            if len(cmdDetectedIndecies) == 0:
                for i in range(len(cmdKeys)):
                    if c == cmdKeys[i][0]:
                        cmdDetectedWord += c
                        cmdDetectedIndecies.append(i)
                if len(cmdDetectedWord) == 0:
                    if isCmd:
                        cmdLine += c
                    else:
                        toPrint += c
            else:
                toEliminate = []
                for i in cmdDetectedIndecies:
                    j = len(cmdDetectedWord)
                    if c != cmdKeys[i][j]:
                        toEliminate.append(i)
                    cmdDetectedWord += c
                
                for x in toEliminate:
                    cmdDetectedIndecies.remove(x)
                
                if len(cmdDetectedIndecies) == 0:
                    if isCmd:
                        cmdLine += cmdDetectedWord
                    else:
                        toPrint += cmdDetectedWord
                    cmdDetectedWord = ""
                else:
                    for i in cmdDetectedIndecies:
                        if len(cmdDetectedWord) == len(cmdKeys[i]):
                            if i == 0:
                                if not isCmd:
                                    isCmd = True
                                else:
                                    cmdLine += cmdDetectedWord
                            elif i == 1:
                                if isCmd:
                                    isCmd = False
                                    cmdQueue.append(cmdLine)
                                    cmdLine = ""
                                else:
                                    toPrint += cmdDetectedWord
                            del cmdDetectedIndecies[:]
                            cmdDetectedWord = ""
                            break
    
    ret = []
    if toPrint:
        ret.append((Commands.PRINT.value, toPrint[0:]))
    for x in cmdQueue:
        ret.append(processCommand(x))
    del cmdQueue[:]
    return ret

