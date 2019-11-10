CFLAGS = `wx-config --cxxflags`
CC = g++

INCLUDES = `python-config --includes`

LIBS = \
	`wx-config --libs std` \
	`python-config --ldflags`

SRCS = \
	process/attribute.cpp \
	gui/actionWidget.cpp \
	gui/port.cpp \
	gui/RobotMonitorApp.cpp \
	gui/RobotMonitorMain.cpp

robotmonitor:
	@ echo Compiling C++
	@ $(CC) $(CFLAGS) $(INCLUDES) -o $@.o $(SRCS) $(LIBS)
	@ echo Compilation success

clean:
	$(RM) robotmonitor.o
	$(RM) -r data

