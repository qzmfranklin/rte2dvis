include ../makevars.mk

#########
all: Utils.o

test: test-utils.exe
	./$< > OUTPUT/test-Utils.txt
	cat OUTPUT/test-Utils.txt

utils.o: utils.cpp
include utils.d

test-utils.exe: test-utils.o utils.o

test-utils.o: test-utils.cpp
include test-utils.d

#########

