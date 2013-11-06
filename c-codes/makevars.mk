# The template Makefile header in subdirectories:
##NOINLINEOPT := True 		# Disable automatic function inlining.
##UNIVERSAL_BINARY := True	# Produce universal binary -fPIC.
## Suppress warning messages about deprecated or antiquated headers.
#CFLAGS 	:= -Wno-deprecated	# additional options for the directory
#include ../makevars.mk
## The MKL library is already linked. Need not to be specified.
##INCS	:= ${INCS} 
##LIBS	:= ${LIBS}
###############################################################################
# Header file and binary file directories 
INCLUDE	:= ./include
BIN	:= ./bin 
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
CFLAGS 	:= ${CFLAGS} -Wall -O3				\
	${FPIC}						\
	${NOINLINEOPT}					\
	-prec-div -no-ftz				\
	-iquote ${INCLUDE}				\
	-restrict
CFLAGSXX:= ${CFLAGSXX} ${CFLAGS}
# Intel Math Kernel Library
MKL_INCS:= 
MKL_LIBS:= -mkl
# MathLink include and library
ML_DIR	:= /Applications/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/MacOSX-x86-64/CompilerAdditions/
ML_INCS	:= -I${ML_DIR}
ML_LIBS	:= -L${ML_DIR} -lMLi3 -lstdc++ -framework Foundation
MPREP	:= ${ML_DIR}/mprep
# LibraryLink include and library
MLL_INCS:= -I/Applications/Mathematica.app/SystemFiles/IncludeFiles/C/
MLL_LIBS:= -L/Applications/Mathematica.app/SystemFiles/Libraries/MacOSX-x86-64/
# The default empty include directories and 
# linking libraries for specific directories
INCS	:= ${MKL_INCS}
LIBS	:= ${MKL_LIBS}
###############################################################################
HDRFILES := $(shell find . -type f -name "*.h") $(shell find . -type f -name "*.h")
CFILES	 := $(shell find . -type f -name "*.c")
CPPFILES := $(shell find . -type f -name "*.cpp")
TSTFILES := $(shell find . -type f -name "test.cpp")
SRCFILES := $(filter-out ${TSTFILES}, ${CFILES} ${CPPFILES})
OBJFILES := ${CFILES:%.c=%.o} ${CPPFILES:%.cpp=%.o}
DEPFILES := ${CFILES:%.c=%.d} ${CPPFILES:%.cpp=%.d}
AUXFILES := 
ALLFILES := ${SRCFILES} ${HDRFILES} ${AUXFILES}
###############################################################################
#.PHONY:
.PHONY: all 						\
	clean cleanx cleanxx 				\
	dist						\
	check						\
	test						\
	install uninstall				\
	todolist

all: ${OBJFILES}
	@make clean
	@echo ${OBJFILES}
clean:
	@rm -rf *_tm.c *.d*
cleanx: 
	@rm -rf *.s DEBUG/* OUPUT/*
cleanxx: clean
	@rm -rf *.dylib *.so *.a *.exe a.out *.o
dist:
	@tar czf pdclib.tgz ${ALLFILES}
install: all
	mv ${OBJFILES} ${BIN}
todolist:
	-@for file in ${ALLFILES:Makefile=};		\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED	\
	$$file;	done; true 
############################################################################### 
.SUFFIXES:
.SUFFIXES: .tm .c .cpp .o .exe .s .d
# C sources are 
%.o: %.c
	@echo Compiling $@...
	@${CC} -c $< -MD -MP ${CFLAGS} ${INCS}
%.s: %.c 
	@echo Generating source-commented assembly list $@...
	@${CC} -S $< ${CFLAGS} -fsource-asm ${INCS}

# CXX sources
%.o: %.cpp
	@echo Compiling $@...
	@${CXX} -c $< -MD -MP ${CFLAGSXX} ${INCS}
%.s: %.cpp
	@echo Generating source-commented assembly list $@...
	@${CXX} -S $< ${CFLAGSXX} -fsource-asm ${INCS} 

# CXX linking at the top level
%.exe: %.o 
	@echo Linking executable $@...
	@${CXX} ${filter %.o,$^} -o $@ ${INCS} ${LIBS} 
%.dylib: %.o
	@echo Generating source-commented assembly list $@...
	@echo Linking dynamic library $@...
	@${CXX} -dynamiclib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}
%.a: %.o
	@echo Linking static library $@...
	@${CXX} -staticlib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}

