# Comment the following line to enable 
# automatic function inlining
#NOINLINE := True
include ../makevars.mk
INCS	:= ${ML_INCS}
LIBS	:= $(MKL_LIBS) ${ML_LIBS} 
############################################################################### 
# This section are the phony targets
all: test-dunavant-rule.exe DunavantRule.exe
obj: test-dunavant-rule.o DunavantRule.o dunavant-rule.o
install: DunavantRule.exe
	cp -rf $< ../ML-exe/$<
	make cleanxx
uninstall: 
	rm -rf ../ML-exe/DunavantRule.exe
	make clean
test: test-dunavant-rule.exe
	./test-dunavant-rule.exe > log
	make clean
###############################################################################
# Directory-specific targets
test-dunavant-rule.exe: dunavant-rule.o test-dunavant-rule.o
DunavantRule.exe: DunavantRule.o DunavantRule_tm.o dunavant-rule.o
DunavantRule_tm.c: DunavantRule.tm
	${MPREP} $< -o $@ 

test-dunavant-rule.o: test-dunavant-rule.c
include test-dunavant-rule.d

dunavant-rule.o: dunavant-rule.c
include dunavant-rule.d

DunavantRule.o: DunavantRule.c
include DunavantRule.d

DunavantRule_tm.o: DunavantRule_tm.c
include DunavantRule_tm.d
