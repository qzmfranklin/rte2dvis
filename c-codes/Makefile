###############################################################################
#  A set of template files to organize projects with a single Makefile
#  Copyright (C) 2013 Zhongming Qu <qzmfrank@umich.edu>
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
###############################################################################
#	Welcome! The comments in this file will guide you through the process.
#  Please read all the comments very carefully before changing anything.
#  	The idea of this set of template files (Makefile, Makefile.vars, and
#  all the makevars.mk) is to help organize any C/C++ project with a single
#  Makefile. 
#  	Usually the advantage of a single Makefile is speed and easy parallel
#  make. The usual drawbacks are bad maintainability and modularity. This 
#  set of files attempts to cut down all the repetitive work that are usually 
#  done manually, i.e., listing target-prerequisites, while preserving almost 
#  the same degree of modularity as that can be achieved through the recursive 
#  make approach.
#  	If you have any suggestions and/or ideas, please let me know.
###############################################################################
#				STEP 1
#PROJECT NAME, VERSION, AND DATE INFORMATION.
#	Be careful! Do NOT accidentally insert some spaces!
# ${TARBALL_NAME} is the name of the tarball generated when one enters
# 	$ make dist
PROJECT_NAME	:=rte2dvis
VERSION		:=v0.02
DATE_TIME	:=$(shell date "+%Y-%m-%d")
RELEASE_NAME	:=${PROJECT_NAME}-${VERSION}-${DATE_TIME}
###############################################################################
#				STEP 2
#QUIET MODE
#	One can comment out the following line to get into verbose mode. Note,
#  however, this flag does NOT control the directory specific building rules.
#  If one wishes to turn on the verbose flag for a specific subdirectoy, one 
#  needs to go to that subdirectory and modify the QUIET flag in that 
#  makevars.mk. This is done so to help debugging the build system itself.
#  	Usually no need to turn to the verbose mode.
QUIET		:=@
###############################################################################
#				STEP 3
#PROJECT STRUCTURAL DIRECTORIES
#	This part follows the usual convention of GNU projects. Modify it only
#  when one has very strong reasons.
SRC		:=src
BIN		:=bin
INCLUDE		:=include
EXAMPLE		:=example# Data files for various tests
BUILD		:=build# Out-of-source build - object and dependency files
DEBUG		:=DEBUG# Profiling and assembly files for debugging use
OUTPUT		:=OUTPUT# Testing result files
STRUCTURAL_DIRS	:=${BIN} ${BUILD} ${DEBUG} ${OUTPUT}
###############################################################################
#				STEP 4
#PROJECT-WIDE COMMON COMPILATION OPTIONS
#	The author of this set of template files is using a MacBookPro. If one 
#  needs to build the same project in a different architecture, e.g., Linux or
#  other Unix-like system, one would most probably only need to modify the 
#  compiling options and linking libraries below. Of course, if one has some
#  architecture-dependent options in some subdirectories, he/she also needs to
#  modify those parts accordingly.
CC		:=icc
CXX		:=icpc
# Compiling options for .c files:
CFLAGS 		:=-O3						\
		-Wall						\
		-prec-div -no-ftz				\
		-MMD -MP					\
		-std=c99					\
		-restrict					\
		-Wno-deprecated

		# 						\
		-DNDEBUG					\
		-fPIC						\
		-fno-inline-functions				\
		-Wno-deprecated					\

CXXFLAGS	:=${CFLAGS}# Compilig options for .cpp files.
ASMFLAGS	:=-S -fsource-asm# Generate source code commented assembly code.
###############################################################################
#				STEP 5
#PROJECT-WIDE INCLUDE AND LINKAGE OPTIONS
#	This part shall also be modified in accordance to the previous part.
#	Intel Math Kernel Library for MacOSX-x86-64
MKL_INCS	:=
MKL_LIBS	:=-mkl
#	OpenMPI Library
MPI_INCS	:='mpiCC -showme:compile'
MPI_LIBS	:='mpiCC -showme:link'
#	MathLink for MacOSX-x86-64
ML_DIR		:=/Applications/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/MacOSX-x86-64/CompilerAdditions/
ML_INCS		:=-I${ML_DIR}
ML_LIBS		:=-L${ML_DIR} -lMLi3 -lstdc++ -framework Foundation
MPREP		:=${ML_DIR}/mprep
#	LibraryLink for MacOSX-x86-64
MLL_INCS	:=-I/Applications/Mathematica.app/SystemFiles/IncludeFiles/C/
MLL_LIBS	:=-L/Applications/Mathematica.app/SystemFiles/Libraries/MacOSX-x86-64/
#PROJECT-WIDE DEFAULT LINKING LIBRARIES AND INCLUDE DIRECTORIES
INCS		:=${MKL_INCS} -iquote ${INCLUDE}
LIBS		:=${MKL_LIBS} 
##	LONESTAR COMPILING OPTIONS
#CFLAGS 	:= -O3 -prec-div -no-ftz -restrict -Wshadow -MMD -MP

#MPIINC 	:= `mpiCC -showme:compile`
#FFTWINC := -I $$FFTW_INC
#MKLINC 	:= -I $$MKL_INC

#MKLLIBS := -L$$MKL_LINK -lmkl_intel_lp64 -lmkl_sequential -lmkl_core 	\
	#-lpthread
#MKLTHRD := -L$$MKL_LINK -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core 	\
	#-lpthread -lm
#FFTWLIB :=  -L $$FFTW_LINK -lfftw3
#MPILIBS := `mpiCC -showme:link`
###############################################################################
-include Makefile.vars# The top level .vars file
-include ${MKVFILES}# The makevars.mk's in the subdirectories
-include ${DEPFILES}# Dependency files (*.d files)
DISTFILES:=${SRCFILES} ${HDRFILES} ${MANFILES} ${EXPFILES} ${MKVFILES} ${MAKFILES}
###############################################################################
#			TOP LEVEL PHONY TARGETS
.PHONY: all 							\
	test							\
	asm							\
	list							\
	clean cleanx cleanxx 					\
	dist distclean						\
	check							\
	todolist						\
	mkdir							\
	tags

all:	${TARGET_ALL} 
test:	${TARGET_TEST} 
asm:	${TARGET_ASM} 
list:	${TARGET_LIST} 
clean:
	@echo "Deleting ${REV_RED}${BUILD}/*${NONE}..."
	${QUIET}rm -rf ${BUILD}/*
cleanx:
	@echo "Deleting ${REV_GREEN}${OUTPUT}/*${NONE}and ${REV_GREEN}${DEBUG}/*${NONE}..."
	${QUIET}rm -rf ${OUTPUT}/* ${DEBUG}/*
cleanxx: clean
	@echo "Deleting ${REV_BLUE}${BIN}/*${NONE}..."
	${QUIET}rm -rf ${BIN}/*
dist:
	@echo Compressing into "${B_CYAN}${RELEASE_NAME}.tgz${NONE}"...
	${QUIET}rm -f ${RELEASE_NAME}.tgz; tar czf ${RELEASE_NAME}.tgz ${DISTFILES}
check:
	#TODO
	@echo Checking distribution integrity:
	${QUIET}echo ${DISTFILES}
todolist: 
	-@for file in ${DISTFILES:=};				\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED		\
	$$file;	done; true 
mkdir: 
	@echo Creating structurual directories:
	@$(foreach dir,${STRUCTURAL_DIRS},if [ ! -d ${dir} ]; 	\
		then echo Creating "${GREEN}${dir}/${NONE}"...; \
		mkdir ${dir}; else echo Already exists:		\
		"${GREY}${dir}${NONE}"; fi;)
rmdir: 
	@echo "${B_RED}WARNING: Deleting ${STRUCTURAL_DIRS}${NONE}"
	${QUIET}rm -rf ${STRUCTURAL_DIRS}
tags:
	@echo Generating tags file for vim...
	${QUIET}ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
###############################################################################
