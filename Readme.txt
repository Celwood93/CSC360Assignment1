Cameron Elwood
V00152812
Assignment 1
CSC 360

Files needed:
PMan.c
inf.c (or any file that runs indefinetly)
Makefile

To run:

make sure all files are in same directory, except for inf.

run "make" in the command line

(if using the inf.c file):
run "gcc inf.c -o inf" in the command line

type "./PMan" in the command line

enter "bg [a executable file (non executable will result in error handling) such as "inf". Either type inf if its in the home directory, or /path/to/infFile/inf if its not in the directory] [whatever paramaters are required by the program, for inf, its a string and an integer, representing a string being printed every time the integers value in seconds passes]" in the command line

the last command can be done up to 35 times, after which an error message will print saying there are too many processes

enter "bglist" in the command line to see the current processes and get the pid of one of them

enter "pstat [pid from above]" in the command line to get information about the pid

enter "bgstop [pid from above]" in the command line to pause the process

enter "bgstart [pid from above]" in the command line to start the process

enter "bgkill [pid from above]" in the command line to terminate the process

enter "bglist" in the command line to see that the process is no longer running

close the command line to leave PMan, or type ctrl c