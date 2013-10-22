# Comment the following line to enable 
# automatic function inlining
#NOINLINE := True
include ../makevars.mk
#INCS	:= ${ML_INCS}
#LIBS	:= $(MKL_LIBS) $(ML_LIBS) 
############################################################################### 
# This section are the phony targets
all: test.exe
test: test.exe
	./test.exe Makefile test02.log example.msh
	make cleanxx
###############################################################################
# Directory-specific targets
test.exe: test.o file-io.o 

test.o: test.c
include test.d

file-io.o: file-io.c
include file-io.d
