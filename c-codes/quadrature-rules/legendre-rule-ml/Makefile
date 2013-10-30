# Comment the following line to enable 
# automatic function inlining
#NOINLINE := True
include ../../makevars.mk
#INCS	:= ${ML_INCS}
LIBS	:= $(MKL_LIBS) #$(ML_LIBS) 
############################################################################### 
# This section are the phony targets
all: legendre-rule.o

test: test-legendre-rule.exe
	rm -rf log
	./$<
###############################################################################
# Directory-specific targets
test-legendre-rule.exe: test-legendre-rule.o legendre-rule.o 

test-legendre-rule.o: test-legendre-rule.c
include test-legendre-rule.d

legendre-rule.o: legendre-rule.c
include legendre-rule.d
