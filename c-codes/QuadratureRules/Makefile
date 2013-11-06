CFLAGS 	:=-Wno-deprecated
include ../makevars.mk
############################################################################### 
# This section are the phony targets
#all: 	DunavantRule.o 			\
	#WandzuraRule.o			\
	#GaussQuadratures.o		

test: 	test.exe
	@./$< > DEBUG/test.txt
	@echo DEBUG information in the ./DEBUG directory
###############################################################################
# Directory-specific targets 
test.exe: test.o DunavantRule.o WandzuraRule.o GaussQuadratures.o
