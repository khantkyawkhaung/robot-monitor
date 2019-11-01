SRCS = wxsmith/*.cpp
CFLAGS = `wx-config --cxxflags`
CC = g++
INCLUDES = `python-config --includes`
LIBS = `wx-config --libs std` `python-config --ldflags`

robotmonitor:
	${CC} ${CFLAGS} ${INCLUDES} -o $@.o ${SRCS} ${LIBS}

clean:
	-rm -f *.o
