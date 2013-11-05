ifdef NOINLINE
	NOINLINEOPT := -fno-inline-functions
endif 
ifdef UNIVERSAL_BINARY
	FPIC := -fPIC
endif 
###############################################################################
CC	:= icc
CPP	:= icpc
CFLAGS 	:= -Wall -O3					\
	-Werror						\
	${FPIC}						\
	${NOINLINEOPT}					\
	-prec-div -no-ftz				\
	-restrict
CFLAGSXX:= 
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
INCS	:=
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
	rm -rf log *.log *.s *.txt OUTPUT
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
# CPP sources
%.d: %.cpp
	@set -e; rm -f $@; \
	${CPP} -M ${CFLAGS} ${INCS} $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o: %.cpp
	${CPP} -c $< ${CFLAGS} ${INCS}
%.s: %.cpp
	${CPP} -S $< ${CFLAGS} ${INCS}


# CPP linking at the top level
%.exe: %.o 
	${CPP} ${filter %.o,$^} -o $@ ${INCS} ${LIBS} 
%.dylib: %.o
	${CPP} -dynamiclib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}
%.a: %.o
	${CPP} -staticlib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}

