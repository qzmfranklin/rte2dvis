include ../makevars.mk
###############################################################################
test: test-file-io.exe dump-msh-file.exe
	@echo Running $^ in DEBUG mode...
	@./test-file-io.exe TimeStamp.cpp 				\
		DEBUG/HelloWorld.txt 					\
		example.msh 						\
		DEBUG/example_out					\
		> DEBUG/OUTPUT.txt
	@./dump-msh-file.exe example.msh DEBUG/test-dump-msh-file	\
		> DEBUG/OUTPUT-dump-msh-file.txt
	@echo Debug results are in DEBUG directory.
###############################################################################
-include ${DEPFILES}
###############################################################################
test-file-io.exe: test-file-io.o file-io.o

dump-msh-file.exe: dump-msh-file.o file-io.o
###############################################################################
