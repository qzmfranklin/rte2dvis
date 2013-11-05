include ../makevars.mk

#########
all: utils.o Table.o file-io.o

test: test.exe test_stat.exe test_table.exe
	./test.exe TimeStamp.cpp HelloWorld.txt example.msh example_out
	./test_stat.exe
	./test_table.exe

cleanx: clean
	rm -rf log *.log *.s *.txt OUTPUT *.info *.nodes *.triangles

utils.o: utils.cpp
include utils.d

Table.o: Table.cpp
include Table.d

test_stat.o: test_stat.cpp
include test_stat.d

test_stat.exe: test_stat.o

test_table.o: test_table.cpp
include test_table.d

test_table.exe: test_table.o Table.o

file-io.o: file-io.c
include file-io.d

test.exe: test.o file-io.o

test.o: test.c
include test.d
#########

