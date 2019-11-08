import atexit
import enum
import os

class ErrorLevel(enum.Enum):
    DEFAULT = 0
    WARNING = 1
    FATAL = 2


logfile = None

def closeLogFile():
    if logfile != None:
        try:
            logfile.close()
        except IOError as e:
            pass

atexit.register(closeLogFile)


def setLogFile(f):    
    dirname = os.path.dirname(f)
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    
    global logfile
    try:
        logfile = open(f, 'w', 1)
    except IOError as e:
        logfile = None

def log(s, lvl=ErrorLevel.DEFAULT):
    if logfile != None:
        if lvl == ErrorLevel.WARNING:
            logfile.write("WARNING: ")
        elif lvl == ErrorLevel.FATAL:
            logfile.write("ERROR: ")
        logfile.write(s)
        logfile.write('\n')


