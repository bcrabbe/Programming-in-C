
CFLAGS = -O3 -Wall -pedantic -std=c99 
TARGET = pegs

SOURCES = pegs.c\
          list.c\
	  compressBoard.c\
          graphics.c\
	  history.c
INCS = pegs.h

LIBS =  -lm `sdl2-config --cflags --libs` 
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)
clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
