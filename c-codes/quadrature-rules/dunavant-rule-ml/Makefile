#NOINLINEOPT := True 		# Disable automatic function inlining.
#UNIVERSAL_BINARY := True	# Produce universal binary -fPIC.
# Suppress warning messages about deprecated or antiquated headers.
CFLAGS 	:=  -Wno-deprecated	
include ../../makevars.mk
#The MKL library is already linked. Need not to be specified.
#INCS	:= ${INCS} ${MLL_INCS} 
#LIBS	:= ${LIBS} ${MLL_LIBS}
############################################################################### 
# This section are the phony targets
all: DunavantRule.o
test: test-DunavantRule.exe
	./test-DunavantRule.exe > DEBUG/test-DunavantRule.txt
	@echo DEBUG information in the ./DEBUG directory
###############################################################################
# Directory-specific targets
test-DunavantRule.exe: DunavantRule.o test-DunavantRule.o

test-DunavantRule.o: test-DunavantRule.cpp
include test-DunavantRule.d

DunavantRule.o: DunavantRule.cpp
include DunavantRule.d
