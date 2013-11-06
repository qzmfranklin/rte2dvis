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

test: 	test-DunavantRule.exe 		\
	test-GaussQuadratures.exe	\
	test-WandzuraRule.exe
	./test-DunavantRule.exe > DEBUG/test-DunavantRule.txt
	./test-WandzuraRule.exe > DEBUG/test-WandzuraRule.txt
	./test-GaussQuadratures.exe > DEBUG/test-GaussQuadratures.txt
	@echo DEBUG information in the ./DEBUG directory
###############################################################################
# Directory-specific targets

########################
test-DunavantRule.exe: DunavantRule.o test-DunavantRule.o

test-DunavantRule.o: test-DunavantRule.cpp
include test-DunavantRule.d

DunavantRule.o: DunavantRule.cpp
include DunavantRule.d

########################
test-WandzuraRule.exe: WandzuraRule.o test-WandzuraRule.o

test-WandzuraRule.o: test-WandzuraRule.cpp
include test-WandzuraRule.d

WandzuraRule.o: WandzuraRule.cpp
include WandzuraRule.d

########################
test-GaussQuadratures.exe: GaussQuadratures.o test-GaussQuadratures.o

test-GaussQuadratures.o: test-GaussQuadratures.cpp
include test-GaussQuadratures.d

GaussQuadratures.o: GaussQuadratures.cpp
include GaussQuadratures.d
