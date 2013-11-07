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
	-iquote ${INCLUDES}				\

CFLAGSXX:= ${CFLAGS}
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
INCLUDES:= $(foreach dir, $(shell find . -type d -name "include"), -I${dir})
INCS	:= ${INCLUDES} ${MKL_INCS}
LIBS	:= ${MKL_LIBS}
###############################################################################
# Colorful echo!
NONE		:=\033[00m 
RED		:=\033[01;31m
BLUE		:=\033[00;34m
GREEN		:=\033[01;32m
YELLOW		:=\033[01;33m
PURPLE		:=\033[01;35m
CYAN		:=\033[01;36m
LIGHTGRAY	:=\033[00;37m
LRED		:=\033[01;31m
LGREEN		:=\033[01;32m
LYELLOW		:=\033[01;33m
LBLUE		:=\033[01;34m
LPURPLE		:=\033[01;35m
LCYAN		:=\033[01;36m
WHITE		:=\033[01;37m
BOLD		:=\033[1m
UNDERLINE	:=\033[4m
############################################################################### 
.SUFFIXES:
.SUFFIXES: .tm .c .cpp .o .exe .s .d
# C sources are 
%.o: %.c
	@echo Compiling "${PURPLE}$@${NONE}"...
	@${CC} -c $< -o $@ -MD -MP ${CFLAGS} ${INCS}
%.s: %.c 
	@echo Generating source-commented assembly list "${LRED}$@${NONE}"...
	@${CC} -S $< -o $@ ${CFLAGS} -fsource-asm ${INCS}

# CXX sources
%.o: %.cpp
	@echo Compiling "${PURPLE}$@${NONE}"...
	@${CXX} -c $< -o $@ -MD -MP ${CFLAGSXX} ${INCS}
%.s: %.cpp
	@echo Generating source-commented assembly list "${LRED}$@${NONE}"...
	@${CXX} -S $< -o $@ ${CFLAGSXX} -fsource-asm ${INCS} 

# CXX linking at the top level
%.exe: %.o 
	@echo Linking executable "${RED}$@${NONE}"...
	@${CXX} ${filter %.o,$^} -o $@ ${INCS} ${LIBS} 
%.dylib: %.o
	@echo Linking dynamic library "${RED}$@${NONE}"...
	@${CXX} -dynamiclib ${filter %.o,$^} -o $@ ${INCS} ${LIBS}
%.a: %.o
	@echo Linking static library "${RED}$@${NONE}"...
	@${CXX} -staticlib ${filter %.o,$^} -o $@ ${INCS} ${LIBS} 
