.phony all:
all: PMan

PMan: PMan.c
	gcc PMan.c -g -std=c99 -lreadline -lhistory -o PMan

.PHONY clean:
clean:
	-rm -rf *.o *.exe
