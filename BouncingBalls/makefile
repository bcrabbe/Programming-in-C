#`sdl2-config --cflags`
#LIBS =  `sdl2-config --libs`

CFLAGS =-framework SDL2  -O4 -Wall -pedantic -std=c99 -lm
INCS = neillsdl2.h 
TARGET = basket
SOURCES =  neillsdl2.c $(TARGET).c
LIBS = 
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
