include ../makevars.mk
###############################################################################
test: test.exe mshtodat.exe
	@echo Running "${RED}$^${NONE}"...
	@./test.exe TimeStamp.cpp 				\
		DEBUG/HelloWorld.txt 					\
		example.msh 						\
		DEBUG/example_out					\
		> DEBUG/OUTPUT.txt
	@./mshtodat.exe example.msh DEBUG/test-mshtodat	\
		> DEBUG/OUTPUT-mshtodat.txt
	@echo Test results are in "${GREEN}DEBUG${NONE}" directory.
###############################################################################
-include ${DEPFILES}
###############################################################################
test.exe: test.o file-io.o

mshtodat.exe: mshtodat.o file-io.o
###############################################################################
