# This makefile is for Windows OS, 
# If you are using Linux, you need to change the del command to rm and remove the .exe extension from the OUT variable name.

# Also, the object files are removed after the executable is created.
# If you want to stop this from happening, remove the del $(OBJS) line under "all".

OBJS	= main.o Minesweeper.o
SOURCE	= main.c Minesweeper.c
HEADER	= Minesweeper.h
OUT		= minesweeper.exe
CC		= gcc
FLAGS	= -g -c -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	del $(OBJS)

main.o: main.c
	$(CC) $(FLAGS) main.c

Minesweeper.o: Minesweeper.c
	$(CC) $(FLAGS) Minesweeper.c

clean:
	del $(OBJS) $(OUT)