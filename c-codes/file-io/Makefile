
include ../makevars.mk

#########
all: file-io.o

test: test-file-io.exe 
	./test-file-io.exe TimeStamp.cpp HelloWorld.txt example.msh example_out

cleanx: clean
	rm -rf log *.log *.s *.txt OUTPUT *.info *.nodes *.triangles

file-io.o: file-io.c
include file-io.d

test-file-io.exe: test-file-io.o file-io.o

test-file-io.o: test-file-io.c
include test-file-io.d
#########

