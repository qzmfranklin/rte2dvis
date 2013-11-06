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
all: GaussQuadratures.o
test: test-GaussQuadratures.exe
	./test-GaussQuadratures.exe > DEBUG/test-GaussQuadratures.txt
	@echo DEBUG information in the ./DEBUG directory
###############################################################################
# Directory-specific targets
test-GaussQuadratures.exe: GaussQuadratures.o test-GaussQuadratures.o

test-GaussQuadratures.o: test-GaussQuadratures.cpp
include test-GaussQuadratures.d

GaussQuadratures.o: GaussQuadratures.cpp
include GaussQuadratures.d
