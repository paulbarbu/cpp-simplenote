APP_NAME = simplenote
CC = g++
CFLAGS = -o $(APP_NAME) -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x
DBG_FLAGS = -g
FILES = simplenote.cpp
DBG_FILES = main.cpp

all:
	$(CC) $(CFLAGS) $(FILES)
debug:
	$(CC) $(CFLAGS) $(DBG_FLAGS) $(DBG_FILES) $(FILES)
	
.PHONY: clean
clean:
	rm -rf *.{out,o,i,exe,swp}
	rm -rf *~
	rm -f simplenote
