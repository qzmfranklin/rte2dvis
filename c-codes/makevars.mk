# The template Makefile header in subdirectories:
#include ../makevars.mk
##NOINLINEOPT := True 		# disable automatic function inlining
##UNIVERSAL_BINARY := True	# produce universal binary -fPIC
## The MKL library is already linked. Need not to be specified.
##INCS	:= ${INCS} ${MLL_INCS} 
##LIBS	:= ${LIBS} ${MLL_LIBS}
###############################################################################
ifdef NOINLINE
	NOINLINEOPT := -fno-inline-functions
endif 
ifdef UNIVERSAL_BINARY
	FPIC := -fPIC
endif 
###############################################################################
CC	:= icc
CXX	:= icpc
CFLAGS 	:= -Wall -O3					\
	${FPIC}						\
	${NOINLINEOPT}					\
	-prec-div -no-ftz				\
	-restrict
CFLAGSXX:= ${CFLAGS}
# Intel Math Kernel Library
MKL_INCS:= 
MKL_LIBS:= -mkl 
# MathLink include and library
ML_DIR	:= /Applications/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/MacOSX-x86-64/CompilerAdditions
ML_INCS	:= -I${ML_DIR}
ML_LIBS	:= -L${ML_DIR} -lMLi3 -lstdc++ -framework Foundation 
MPREP	:= ${ML_DIR}/mprep
# LibraryLink include and library
MLL_INCS:= -I/Applications/Mathematica.app/SystemFiles/IncludeFiles/C 
MLL_LIBS:= -L/Applications/Mathematica.app/SystemFiles/Libraries/MacOSX-x86-64/
# The default empty include directories and 
# linking libraries for specific directories
INCS	:= ${MKL_INCS}
LIBS	:= ${MKL_LIBS}
###############################################################################
#.PHONY:
.PHONY: all 						\
	clean cleanx cleanxx 				\
	install uninstall				\
	test
clean:
	rm -rf *.o *_tm.c a.out *.d*
cleanx: clean
	rm -rf log *.log *.s *.txt DEBUG/* OUPUT/*
cleanxx: clean
	rm -rf *.dylib *.so *.a *.exe a.out 
############################################################################### 
.SUFFIXES:
.SUFFIXES: .tm .c .cpp .o .exe .s .d
# C sources are 
%.d: %.c
	@set -e; rm -f $@; \
	${CC} -M ${CFLAGS} ${INCS} $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o: %.c
	${CC} -c $< ${CFLAGS} ${INCS}
%.s: %.c 
	${CC} -S $< ${CFLAGS} ${INCS}
# CXX sources
%.d: %.cpp
	@set -e; rm -f $@; \
	${CXX} -M ${CFLAGSXX} ${INCS} $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o: %.cpp
	${CXX} -c $< ${CFLAGSXX} ${INCS}
%.s: %.cpp
	${CXX} -S $< ${CFLAGSXX} ${INCS}


# CXX linking at the top level
%.exe: %.o 
	${CXX} ${filter %.o,$^} -o $@ ${INCS} ${LIBS} 
%.dylib: %.o
	${CXX} -dynamiclib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}
%.a: %.o
	${CXX} -staticlib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}

