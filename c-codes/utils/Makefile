# automatic function inlining
#NOINLINE := True
# generat universal binary usig -fPIC instead of fpic
#UNIVERSAL_BINARY := True
include ../makevars.mk
#INCS	:= ${MLL_INCS} ${ML_INCS}
#LIBS	:= ${MLL_LIBS} ${ML_LIBS} ${MKL_LIBS}
############################################################################### 
all: file-io-test.exe

install: file-io-test.exe
	@echo 'do nothing' 
uninstall:
	@echo 'do nothing' 
############################################################################### 
file-io-test.exe: file-io-test.o file-io.o

file-io.o: file-io.c
include file-io.d

file-io-test.o: file-io-test.c
include file-io-test.d
