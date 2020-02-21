Advent of Code 2019

(Note: I did not finish some of the later days)

https://adventofcode.com/2019

I exclusivly used C++ for this year.

The files read in input through a file called "in.txt", but any file can
be used if you pass a different file as the command line arg.

You will probably have to read the source code for the given day you are 
looking for because sometimes I put the input directly in the source 
code if it made more sense to me. I apologize for this inconsistency.

If you define the constant "PARTONE" then the program will give you the
part one solution, otherwise the default is part two. This line will be
left commented out at the start of each source.

Some days I instead just broke up the program into two files labeled 
with "a" or "b". Once again sorry for the inconsistency.

The output files are always .exe, even on linux. I did this because it 
made the makefile a bit cleaner.

Example: compile and run day 25 with

$ make 25.exe
$ ./25.exe
