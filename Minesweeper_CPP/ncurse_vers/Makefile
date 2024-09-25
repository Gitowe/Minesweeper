# This makefile is for Windows OS, 
# If you are using Linux, you need to change the del command to rm and remove the .exe extension from the OUT variable name.

# Also, the object files are removed after the executable is created.
# If you want to stop this from happening, remove the del $(OBJS) line under "all".

# Fair warning, the -I and -L flags are hardcoded to my mingw64 installation path.
# You will need to change these to your own path if you are using mingw64.
# I wish ncurses worked better, but it doesn't. :(

OBJS	= main.o Minesweeper.o
SOURCE	= main.cpp Minesweeper.cpp
HEADER	= Minesweeper.h
OUT		= minesweeper.exe
CC		= g++
FLAGS	= -g -c -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) -I C:\msys64\mingw64\include -L C:\msys64\mingw64\lib -lncurses
	del $(OBJS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -I C:\msys64\mingw64\include -L C:\msys64\mingw64\lib -lncurses

Minesweeper.o: Minesweeper.cpp
	$(CC) $(FLAGS) Minesweeper.cpp -I C:\msys64\mingw64\include -L C:\msys64\mingw64\lib -lncurses

clean:
	del $(OBJS) $(OUT)