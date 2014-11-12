CFLAGS =  -O4 -Wall -pedantic -std=c99
INCS =
TARGET = wordLadder
SOURCES =  $(TARGET).c
LIBS =
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)

	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET)



