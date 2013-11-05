include ../makevars.mk

#########
all: Utils.o

test: test-Utils.exe
	./$< > OUTPUT/test-Utils.txt
	cat OUTPUT/test-Utils.txt

Utils.o: Utils.cpp
include Utils.d

test-Utils.exe: test-Utils.o Utils.o

test-Utils.o: test-Utils.cpp
include test-Utils.d

#########

