# automatic function inlining
#NOINLINE := True
# generat universal binary usig -fPIC instead of fpic
#UNIVERSAL_BINARY := True
include ../makevars.mk
#INCS	:= ${MLL_INCS} ${ML_INCS}
#LIBS	:= ${MLL_LIBS} ${ML_LIBS} ${MKL_LIBS}
############################################################################### 
all: test.exe
test: test.exe
	@echo "###############################################################################"
	./$< Makefile MathLink
	@echo "###############################################################################"
	make cleanxx
############################################################################### 
test.exe: test.o file-io.o

test.o: file-io-test.c
include file-io-test.d

file-io.o: file-io.c
include file-io.d 
