################################################################################
#  A set of template files to build projects with a single Makefile
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
################################################################################
#				STEP 1
#	DIRECTORY NAME FROM THE ROOT DIRECTORY
DIR001:=src/utils# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR001}QUIET:=@
###############################################################################
#				STEP 2
#	DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
${DIR001}CXXFLAGS:=${CXXFLAGS}
${DIR001}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR001}CPPFILES:=utils.cpp StatVector.cpp Table.cpp TimeStamp.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR001}TSTEXE:= test_mkl_solvers.exe

${BIN}/test_mkl_solvers.exe: ${OBJ}/test_mkl_solvers.o ${OBJ}/utils.o
###############################################################################
#	Congratulations! You have completed everything you need to do to build
#  this directory. You do not need to modify this file unless some C and/or
#  CPP source files are added, removed, or renamed. In that case, just go over
#  STEP 1-5 again and it is done!
###############################################################################


































###############################################################################
###############################################################################
###############################################################################
###############################################################################
#	The following four blocks of codes generate proper directory-specific
#  variables and add proper files to the top level variables. Please anyway do
#  NOT modify them.
################## DO NOT MODIFY ################
${DIR001}CPPFILES:=${${DIR001}CPPFILES:%=${DIR001}/%}
${DIR001}OBJFILES:=${${DIR001}CPPFILES:${DIR001}%.cpp=${OBJ}%.o}
${DIR001}DEPFILES:=${${DIR001}OBJFILES:%.o=%.d}
${DIR001}ASMFILES:=${${DIR001}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR001}BINEXE:=${${DIR001}BINEXE:%=${BIN}/%}
${DIR001}BINCPP:=${${DIR001}BINEXE:${BIN}/%.exe=${DIR001}/%.cpp}
${DIR001}BINOBJ:=${${DIR001}BINCPP:${DIR001}%.cpp=${OBJ}%.o}
${DIR001}BINDEP:=${${DIR001}BINOBJ:%.o=%.d}
${DIR001}BINASM:=${${DIR001}BINOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR001}TSTEXE:=${${DIR001}TSTEXE:%=${BIN}/%}
${DIR001}TSTCPP:=${${DIR001}TSTEXE:${BIN}/%.exe=${DIR001}/%.cpp}
${DIR001}TSTOBJ:=${${DIR001}TSTCPP:${DIR001}%.cpp=${OBJ}%.o}
${DIR001}TSTDEP:=${${DIR001}TSTOBJ:%.o=%.d}
${DIR001}TSTASM:=${${DIR001}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR001}CFILES} \
	${${DIR001}CPPFILES} ${${DIR001}TSTCPP} \
	${${DIR001}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR001}DEPFILES} \
	${${DIR001}TSTDEP} ${${DIR001}BINDEP}
################## DO NOT MODIFY ################
###############################################################################
#				WANRING
#  Please do not modify the following part if you are not 100% sure about it.
#  The design of this file is that one only needs to modify the part above.
#  If you find you need to modify any of the parts below, be very careful!
#				WANRING
###############################################################################
###############################################################################

#DIRECTORY-SPECIFIC PATTERN RULES
#
#	C++ linkage at the top level is done in the Makefile.vars in the root 
#  directory. Any special build rules for libraries and executables should be
#  secified in STEP 6 in this makevars.mk file.
#  	Not using "\" to break long lines because that would hurt the non-quiet 
#  mode output aestetics. The new colorful version of "make list" is ready now!

#DIRECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR001}-all ${DIR001}-test \
	${DIR001}-asm ${DIR001}-check \
	${DIR001}-list

#  C++ sources
${OBJ}/%.o: ${DIR001}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR001}QUIET}${CXX} -o $@ -c $< ${${DIR001}CXXFLAGS} ${${DIR001}INCS}
${ASM}/%.s: ${DIR001}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR001}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR001}CXXFLAGS} ${${DIR001}INCS} 

	
TARGET_ALL	:=${TARGET_ALL} ${DIR001}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR001}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR001}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR001}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR001}-list
${DIR001}-all: ${${DIR001}OBJFILES} ${${DIR001}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR001}-test: ${${DIR001}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR001}-asm: ${${DIR001}ASMFILES} ${${DIR001}TSTASM} \
	${${DIR001}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR001}-check: ${${DIR001}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR001}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}$@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR001}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR001}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR001}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR001}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR001}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR001}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
