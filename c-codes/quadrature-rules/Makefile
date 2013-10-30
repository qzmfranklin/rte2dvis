# Comment the following line to enable 
# automatic function inlining
#NOINLINE := True
include ../makevars.mk
#INCS	:= ${ML_INCS}
LIBS	:= ${MKL_LIBS}	# $(ML_LIBS) 
############################################################################### 
# This section are the phony targets
all: file-io.o dump-msh-file.exe
test: test.exe dump-msh-file.exe
	./test.exe Makefile test02.log example.msh dump_msh_example
	./dump-msh-file.exe example.msh
cleanx: clean
	rm -rf log *.log *.s *.info *.nodes *.triangles
###############################################################################
# Directory-specific targets
test.exe: test.o file-io.o 

dump-msh-file.exe: dump-msh-file.o file-io.o

dump-msh-file.o: dump-msh-file.c

test.o: test.c				
include test.d

file-io.o: file-io.c			
include file-io.d
