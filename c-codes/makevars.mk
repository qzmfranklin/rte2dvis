###############################################################################
HDRFILES:= $(shell find . -type f -name "*.h") # Header
TSTFILES:= $(shell find . -type f -name "test.cpp") # Test
CFILES	:= $(shell find . -type f -name "*.c") # C Source
CPPFILES:= $(filter-out ${TSTFILES}, $(shell find . -type f -name "*.cpp")) # CPP Source
SRCFILES:= ${CFILES} ${CPPFILES} # Source
OBJFILES:= ${CFILES:%.c=%.o} ${CPPFILES:%.cpp=%.o} # Object
DEPFILES:= ${OBJFILES:%.o=%.d} # Dependency
ASMFILES:= ${OBJFILES:%.o=%.s} # Assembly
DOCFILES:= # Documentation
EXPFILES:= # Example
MANFILES:= # Manual
AUXFILES:= ${DOCFILES} ${EXPFILES} ${MANFILES} # Auxilliary
ALLFILES:= ${SRCFILES} ${HDRFILES} ${AUXFILES} # All Files for Distribution
BINFILES:= ${TSTFILES:%.cpp=%.exe} ./file-io/mshtodat.exe # All EXE files
###############################################################################
CC	:= icc
CXX	:= icpc
CFLAGS 	:= -O3						\
	-Wall						\
	-Wno-deprecated					\
	-prec-div -no-ftz				\
	-restrict
	# 						\
	-fPIC						\
	-fno-inline-functions				\
	-iquote ${}					\

CFLAGSXX:= ${CFLAGS}

# Intel Math Kernel Library
MKL_INCS:= 
MKL_LIBS:= -mkl
# MathLink 
ML_DIR	:= /Applications/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/MacOSX-x86-64/CompilerAdditions/
ML_INCS	:= -I${ML_DIR}
ML_LIBS	:= -L${ML_DIR} -lMLi3 -lstdc++ -framework Foundation
MPREP	:= ${ML_DIR}/mprep
# LibraryLink 
MLL_INCS:= -I/Applications/Mathematica.app/SystemFiles/IncludeFiles/C/
MLL_LIBS:= -L/Applications/Mathematica.app/SystemFiles/Libraries/MacOSX-x86-64/
# Project include directories
INCLUDES:= $(foreach dir, $(shell find . -type d -name "include"), -I${dir})
# The default empty include directories and 
# linking libraries for specific directories
INCS	:= ${INCLUDES} ${MKL_INCS}
LIBS	:= ${MKL_LIBS}
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
.SUFFIXES:
.SUFFIXES: .tm .c .cpp .o .exe .s .d
# C sources are 
%.o: %.c
	@echo Compiling "${B_RED}$@${NONE}"...
	@${CC} -o $@ -c $< -MD -MP ${CFLAGS} ${INCS}
%.s: %.c 
	@echo Generating source-commented assembly list "${BROWN}$@${NONE}"...
	@${CC} -o $@ -S $< ${CFLAGS} -fsource-asm ${INCS}

# CXX sources
%.o: %.cpp
	@echo Compiling "${B_RED}$@${NONE}"...
	@${CXX} -o $@ -c $< -MD -MP ${CFLAGSXX} ${INCS}
%.s: %.cpp
	@echo Generating source-commented assembly list "${BROWN}$@${NONE}"...
	@${CXX} -o $@ -S $< ${CFLAGSXX} -fsource-asm ${INCS} 

# CXX linking at the top level
%.exe: %.o 
	@echo Linking executable "${B_MAGENTA}$@${NONE}"...
	@${CXX} -o $@ ${filter %.o,$^} ${LIBS} 
%.dylib: %.o
	@echo Linking dynamic library "${B_MAGENTA}$@${NONE}"...
	@${CXX} -o $@ -dynamiclib ${filter %.o,$^} ${LIBS}
%.a: %.o
	@echo Linking static library "${B_MAGENTA}$@${NONE}"...
	@${CXX} -o $@ -staticlib ${filter %.o,$^} ${LIBS} 
###############################################################################
-include ${DEPFILES}
###############################################################################
.PHONY: all 						\
	clean cleanx cleanxx 				\
	dist						\
	distclean					\
	check						\
	test						\
	install uninstall				\
	todolist

all: ${OBJFILES}

clean:
	@rm -rf 
cleanx: 
	@rm -rf ${ASMFILES} 				\
		${TSTFILES:%.cpp=%.s}
cleanxx: clean
	@rm -rf ${OBJFILES} ${DEPFILES} ${BINFILES}	\
		${TSTFILES:%.cpp=%.o} ${TSTFILES:%.cpp=%.d}
dist:
	@echo Compressing into "${B_CYAN}${TARBALL_NAME}${NONE}"...
	@tar czf ${TARBALL_NAME} ${ALLFILES}
distclean:
	@rm -rf *.tgz
test: ${TSTFILES:%.cpp=%.exe}
	@echo Test information is in "${B_GREEN}DEBUG/${NONE}".  
check:
	@echo "${B_BROWN}INCLUDES${NONE}" = ${INCLUDES}
	@echo "${B_BROWN}TSTFILES${NONE}" = ${TSTFILES}
	@echo "${B_BROWN}SRCFILES${NONE}" = ${SRCFILES}
	@echo "${B_BROWN}OBJFILES${NONE}" = ${OBJFILES}
	@echo "${B_BROWN}DEPFILES${NONE}" = ${DEPFILES}
	@echo "${B_BROWN}BINFILES${NONE}" = ${BINFILES}
install: all
	mv ${OBJFILES} ${BIN}
todolist:
	-@for file in ${ALLFILES:Makefile=};		\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED	\
	$$file;	done; true 
###############################################################################
