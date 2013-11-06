#NOINLINEOPT := True 		# Disable automatic function inlining.
#UNIVERSAL_BINARY := True	# Produce universal binary -fPIC.
# Suppress warning messages about deprecated or antiquated headers.
CFLAGS 	:=  -Wno-deprecated	
include ../makevars.mk
#The MKL library is already linked. Need not to be specified.
#INCS	:= ${INCS} ${MLL_INCS} 
#LIBS	:= ${LIBS} ${MLL_LIBS}
############################################################################### 
# This section are the phony targets
all: 	DunavantRule.o 			\
	WandzuraRule.o			\
	GaussQuadratures.o		

test: 	test.exe
	./$< > DEBUG/test.txt
	@echo DEBUG information in the ./DEBUG directory
###############################################################################
# Directory-specific targets 
test.exe: test.o DunavantRule.o WandzuraRule.o GaussQuadratures.o

DunavantRule.o: DunavantRule.cpp
include DunavantRule.d

WandzuraRule.o: WandzuraRule.cpp
include WandzuraRule.d

GaussQuadratures.o: GaussQuadratures.cpp
include GaussQuadratures.d
