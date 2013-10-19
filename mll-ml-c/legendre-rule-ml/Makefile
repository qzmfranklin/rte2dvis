# Comment the following line to enable 
# automatic function inlining
#NOINLINE := True
include ../makevars.mk
INCS	:= ${ML_INCS}
LIBS	:= $(MKL_LIBS) $(ML_LIBS) 
############################################################################### 
# This section are the phony targets
all: LegendreRule.exe
obj: LegendreRule.o legendre-rule.o
install: LegendreRule.exe
	cp -rf $< ../ML-exe/$<
	make cleanxx
uninstall: 
	rm -rf ../ML-exe/LegendreRule.exe
	make clean
###############################################################################
# Directory-specific targets
LegendreRule.exe: LegendreRule.o LegendreRule_tm.o legendre-rule.o
LegendreRule_tm.c: LegendreRule.tm
	${MPREP} $< -o $@ 

LegendreRule_tm.o: LegendreRule_tm.c
include LegendreRule_tm.d

LegendreRule.o: LegendreRule.c
include LegendreRule.d

legendre-rule.o: legendre-rule.c
include legendre-rule.d
