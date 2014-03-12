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
DIR003:=${SRC}/solver# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR003}QUIET:=@
###############################################################################
#				STEP 2
#	DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
${DIR003}CXXFLAGS:=${CXXFLAGS}
${DIR003}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR003}CPPFILES:=solver_v1.cpp solver_v2.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR003}BINEXE:=rte2dvisv1.exe

${BIN}/rte2dvisv1.exe: ${OBJ}/rte2dvisv1.o ${OBJ}/solver_v1.o \
	${OBJ}/utils.o ${OBJ}/mshio.o ${OBJ}/dunavant.o ${OBJ}/wandzura.o \
	${OBJ}/gauss.o ${OBJ}/arcsinh.o 
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR003}TSTEXE:= test_solver_v1.exe test_solver_v2.exe

${BIN}/test_solver_v1.exe: ${OBJ}/test_solver_v1.o ${OBJ}/solver_v1.o \
	${OBJ}/utils.o ${OBJ}/mshio.o ${OBJ}/dunavant.o ${OBJ}/wandzura.o \
	${OBJ}/gauss.o ${OBJ}/arcsinh.o

${BIN}/test_solver_v2.exe: ${OBJ}/test_solver_v2.o ${OBJ}/solver_v2.o \
	${OBJ}/utils.o ${OBJ}/mshio.o ${OBJ}/dunavant.o ${OBJ}/wandzura.o \
	${OBJ}/gauss.o ${OBJ}/arcsinh.o ${OBJ}/rmsm.o
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
${DIR003}CPPFILES:=${${DIR003}CPPFILES:%=${DIR003}/%}
${DIR003}OBJFILES:=${${DIR003}CPPFILES:${DIR003}%.cpp=${OBJ}%.o}
${DIR003}DEPFILES:=${${DIR003}OBJFILES:%.o=%.d}
${DIR003}ASMFILES:=${${DIR003}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR003}BINEXE:=${${DIR003}BINEXE:%=${BIN}/%}
${DIR003}BINCPP:=${${DIR003}BINEXE:${BIN}/%.exe=${DIR003}/%.cpp}
${DIR003}BINOBJ:=${${DIR003}BINCPP:${DIR003}%.cpp=${OBJ}%.o}
${DIR003}BINDEP:=${${DIR003}BINOBJ:%.o=%.d}
${DIR003}BINASM:=${${DIR003}BINOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR003}TSTEXE:=${${DIR003}TSTEXE:%=${BIN}/%}
${DIR003}TSTCPP:=${${DIR003}TSTEXE:${BIN}/%.exe=${DIR003}/%.cpp}
${DIR003}TSTOBJ:=${${DIR003}TSTCPP:${DIR003}%.cpp=${OBJ}%.o}
${DIR003}TSTDEP:=${${DIR003}TSTOBJ:%.o=%.d}
${DIR003}TSTASM:=${${DIR003}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR003}CFILES} \
	${${DIR003}CPPFILES} ${${DIR003}TSTCPP} \
	${${DIR003}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR003}DEPFILES} \
	${${DIR003}TSTDEP} ${${DIR003}BINDEP}
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
.PHONY: ${DIR003}-all ${DIR003}-test \
	${DIR003}-asm ${DIR003}-check \
	${DIR003}-list

#  C++ sources
${OBJ}/%.o: ${DIR003}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR003}QUIET}${CXX} -o $@ -c $< ${${DIR003}CXXFLAGS} ${${DIR003}INCS}
${ASM}/%.s: ${DIR003}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR003}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR003}CXXFLAGS} ${${DIR003}INCS} 

	
TARGET_ALL	:=${TARGET_ALL} ${DIR003}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR003}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR003}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR003}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR003}-list
${DIR003}-all: ${${DIR003}OBJFILES} ${${DIR003}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR003}-test: ${${DIR003}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR003}-asm: ${${DIR003}ASMFILES} ${${DIR003}TSTASM} \
	${${DIR003}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR003}-check: ${${DIR003}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR003}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}$@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR003}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR003}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR003}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR003}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR003}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR003}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
