include ../makevars.mk 
############################################################################### 
test: test-utils.exe
	@echo Running $^ in DEBUG mode...
	@./$< > DEBUG/test-Utils.txt
	@echo Debug information in DEBUG/test-Utils.txt.
############################################################################### 
test-utils.exe: test-utils.o utils.o
