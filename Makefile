# Makefile for Borland C++ 4.0 / Turbo C++ 4.0 DOS project

CC = bcc
CFLAGS = -c -O -w-
LIBS = math.lib
OBJS = pcanywhere.obj serial.obj protocol.obj keyboard.obj screen.obj display.obj
PROGRAM = pcanywhere.exe

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -e$(PROGRAM) $(OBJS) $(LIBS)

pcanywhere.obj: pcanywhere.cpp serial.h protocol.h keyboard.h screen.h display.h
	$(CC) $(CFLAGS) pcanywhere.cpp

serial.obj: serial.cpp serial.h
	$(CC) $(CFLAGS) serial.cpp

protocol.obj: protocol.cpp protocol.h serial.h
	$(CC) $(CFLAGS) protocol.cpp

keyboard.obj: keyboard.cpp keyboard.h
	$(CC) $(CFLAGS) keyboard.cpp

screen.obj: screen.cpp screen.h protocol.h
	$(CC) $(CFLAGS) screen.cpp

display.obj: display.cpp display.h
	$(CC) $(CFLAGS) display.cpp

clean:
	del *.obj
	del *.exe
