CC = g++
CFLAGS = `wx-config --cxxflags`

INCLUDES = `python-config --includes`

LIBS = \
	`wx-config --libs std` \
	`python-config --ldflags`

SRCS = \
	gui/actionWidget.cpp \
	gui/logging.cpp \
	gui/RobotMonitorApp.cpp \
	gui/RobotMonitorMain.cpp \
	process/attribute.cpp

robotmonitor:
	@ echo [34mCompiling C++[0m
	@ $(CC) $(CFLAGS) $(INCLUDES) -o $@.o $(SRCS) $(LIBS)
	@ echo [34mCompilation success[0m

run: robotmonitor.o
	@ ./$<

clean:
	@ $(RM) robotmonitor.o
	@ $(RM) -r data

