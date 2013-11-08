###############################################################################
INCLUDE	:=include
BUILD	:=build# Out-of-source build - object, dependency, and assembly files
DEBUG	:=DEBUG# Profiling and debugging files
OUTPUT	:=OUTPUT# Testing result files
############################################################################### 
# Source and build files
CFILES	:=# *.c
CPPFILES:=# *.cpp
OBJFILES:=# *.o
DEPFILES:=# *.d
ASMFILES:=# *.s: source code commented
BINFILES:=# *.exe 
############################################################################### 
# Distribution files
SRCFILES:=# All source files
HDRFILES:=$(shell find . -type f -name "*.h") # All header files 
MAKFILES:=Makefile $(shell find . -type f -name "makevars.mk") # Makefile and related file(s)
DOCFILES:=# Documentation
MANFILES:=# Manuals
EXPFILES:=# Examples
DISTFILES:=${SRCFILES} ${HDRFILES} ${MAKFILES} ${MANFILES} ${EXPFILES}
############################################################################### 
# Testing and debuggin files
#TSTFILES:=
###############################################################################
# Compilation options
CC	:=icc
CXX	:=icpc
CFLAGS 	:=-O3						\
	-Wall						\
	-prec-div -no-ftz				\
	-MD -MP						\
	-restrict
	# 						\
	-fPIC						\
	-fno-inline-functions				\
	-Wno-deprecated					\

CXXFLAGS:=${CFLAGS}

ASMFLAGS:=-S -fsource-asm# Generate source code commented assembly code
###############################################################################
# Include and linkage options
# Intel Math Kernel Library
MKL_INCS:=
MKL_LIBS:=-mkl
# MathLink 
ML_DIR	:=/Applications/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/MacOSX-x86-64/CompilerAdditions/
ML_INCS	:=-I${ML_DIR}
ML_LIBS	:=-L${ML_DIR} -lMLi3 -lstdc++ -framework Foundation
MPREP	:=${ML_DIR}/mprep
# LibraryLink 
MLL_INCS:=-I/Applications/Mathematica.app/SystemFiles/IncludeFiles/C/
MLL_LIBS:=-L/Applications/Mathematica.app/SystemFiles/Libraries/MacOSX-x86-64/
# Project include directories
# The default empty include directories and 
# linking libraries for specific directories
INCS	:=${MKL_INCS} -iquote ${INCLUDE}#$(foreach dir, $(shell find . -type d -name "include"), -iquote ${dir})
LIBS	:=${MKL_LIBS}
###############################################################################
# Colorful shell echo!
NONE		:=\033[00m 
# Normal with color
BLACK		:=\033[00;30m
RED		:=\033[00;31m
GREEN		:=\033[00;32m
BROWN		:=\033[00;33m 
BLUE		:=\033[00;34m
MAGENTA		:=\033[00;35m
CYAN		:=\033[00;36m
GREY		:=\033[00;37m 
# Bold
B_BLACK		:=\033[01;30m
B_RED		:=\033[01;31m
B_GREEN		:=\033[01;32m
B_BROWN		:=\033[01;33m 
B_BLUE		:=\033[01;34m
B_MAGENTA	:=\033[01;35m
B_CYAN		:=\033[01;36m
B_GREY		:=\033[01;37m 
# Underlined
U_BLACK		:=\033[04;30m
U_RED		:=\033[04;31m
U_GREEN		:=\033[04;32m
U_BROWN		:=\033[04;33m 
U_BLUE		:=\033[04;34m
U_MAGENTA	:=\033[04;35m
U_CYAN		:=\033[04;36m
U_GREY		:=\033[04;37m 
# Blinking
BLK_BLACK	:=\033[05;30m
BLK_RED		:=\033[05;31m
BLK_GREEN	:=\033[05;32m
BLK_BROWN	:=\033[05;33m 
BLK_BLUE	:=\033[05;34m
BLK_MAGENTA	:=\033[05;35m
BLK_CYAN	:=\033[05;36m
BLK_GREY	:=\033[05;37m 
# Reversed foreground and background color
REV_BLACK	:=\033[07;30m
REV_RED		:=\033[07;31m
REV_GREEN	:=\033[07;32m
REV_BROWN	:=\033[07;33m 
REV_BLUE	:=\033[07;34m
REV_MAGENTA	:=\033[07;35m
REV_CYAN	:=\033[07;36m
REV_GREY	:=\033[07;37m 
############################################################################### 
# Pattern rules template
# CXX linking at the top level
#%.exe: %.o 
	#@echo Linking executable "${B_MAGENTA}$@${NONE}"...
	#@${CXX} -o $@ ${filter %.o,$^} ${LIBS} 
#%.dylib: %.o
	#@echo Linking dynamic library "${B_MAGENTA}$@${NONE}"...
	#@${CXX} -o $@ -dynamiclib ${filter %.o,$^} ${LIBS}
#%.a: %.o
	#@echo Linking static library "${B_MAGENTA}$@${NONE}"...
	#@${CXX} -o $@ -staticlib ${filter %.o,$^} ${LIBS} 
## C sources are 
#%.o: %.c
	#@echo Compiling "${B_RED}$@${NONE}"...
	#@${CC} -o $@ -c $< -MD -MP ${CFLAGS} ${INCS}
#%.s: %.c 
	#@echo Generating source-commented assembly list "${BROWN}$@${NONE}"...
	#@${CC} -o $@ -S $< ${CFLAGS} -fsource-asm ${INCS} 
## CXX sources
#%.o: %.cpp
	#@echo Compiling "${B_RED}$@${NONE}"...
	#@${CXX} -o $@ -c $< -MD -MP ${CXXFLAGS} ${INCS}
#%.s: %.cpp
	#@echo Generating source-commented assembly list "${BROWN}$@${NONE}"...
	#@${CXX} -o $@ -S $< ${CXXFLAGS} -fsource-asm ${INCS} 
