
CFLAGS = -O4 -Wall -pedantic -std=c99 
TARGET = maze
SRCDIR = ./SOURCES/
SOURCES = $(SRCDIR)genericFunctions.c\
          $(SRCDIR)graphics.c\
	  $(SRCDIR)mazeFromFile.c\
	  $(SRCDIR)mazeGenerator.c\
          $(SRCDIR)mazeSolver.c\
          $(SRCDIR)$(TARGET).c
INCS = $(SRCDIR)$(TARGET).h

LIBS = -framework SDL2 -lm
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
