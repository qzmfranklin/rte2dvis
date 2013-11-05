include ../../makevars.mk

#########


test: test-opt.exe
	./$< -b -a hello 
	./$< -ba

test-opt.exe: test-opt.o
include test-opt.d

#########
