include ../makevars.mk
#NOINLINEOPT := True 		# disable automatic function inlining
#UNIVERSAL_BINARY := True	# produce universal binary -fPIC
# The MKL library is already linked. Need not to be specified.
#INCS	:= ${INCS} ${MLL_INCS} 
#LIBS	:= ${LIBS} ${MLL_LIBS}
###############################################################################
all: file-io.o

test: test-file-io.exe dump-msh-file.exe
	./test-file-io.exe TimeStamp.cpp 				\
		DEBUG/HelloWorld.txt 					\
		example.msh 						\
		DEBUG/example_out					\
		> DEBUG/OUTPUT.txt
	./dump-msh-file.exe example.msh DEBUG/test-dump-msh-file 
###############################################################################
file-io.o: file-io.c
include file-io.d

test-file-io.exe: test-file-io.o file-io.o

test-file-io.o: test-file-io.c
include test-file-io.d

dump-msh-file.exe: dump-msh-file.o file-io.o

dump-msh-file.o: dump-msh-file.c
include dump-msh-file.d
###############################################################################
