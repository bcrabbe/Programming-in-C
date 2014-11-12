CFLAGS =-framework SDL2  -O4 -Wall -pedantic -std=c99 -lm
INCS = 
TARGET = maze
SOURCES = $(TARGET).c
LIBS = 
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) $(INCS) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 

