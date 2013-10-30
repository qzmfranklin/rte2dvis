# Comment the following line to enable 
# automatic function inlining
#NOINLINE := True
include ../../makevars.mk
INCS	:= ${ML_INCS}
LIBS	:= $(MKL_LIBS) ${ML_LIBS} 
############################################################################### 
# This section are the phony targets
all: dunavant-rule.o
test: test-dunavant-rule.exe
	rm -rf log
	./test-dunavant-rule.exe > log
###############################################################################
# Directory-specific targets
test-dunavant-rule.exe: dunavant-rule.o test-dunavant-rule.o

test-dunavant-rule.o: test-dunavant-rule.c
include test-dunavant-rule.d

dunavant-rule.o: dunavant-rule.c
include dunavant-rule.d
