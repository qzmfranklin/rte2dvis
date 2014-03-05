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
DIR002:=${SRC}/mesh# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR002}QUIET:=@
###############################################################################
#				STEP 2
#	DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
${DIR002}CXXFLAGS:=${CXXFLAGS}
${DIR002}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR002}CPPFILES:=mshio.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR002}BINEXE:=dumpmsh.exe

${BIN}/dumpmsh.exe: ${OBJ}/dumpmsh.o
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR002}TSTEXE:= test_mshio.exe

${BIN}/test_mshio.exe: ${OBJ}/test_mshio.o ${OBJ}/mshio.o \
	${OBJ}/utils.o

#${BIN}/test_msh_ray_tracing.exe: ${OBJ}/test_msh_ray_tracing.o \
	#${OBJ}/mshio.o ${OBJ}/utils.o ${OBJ}/msh_ray_tracing.o
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
${DIR002}CPPFILES:=${${DIR002}CPPFILES:%=${DIR002}/%}
${DIR002}OBJFILES:=${${DIR002}CPPFILES:${DIR002}%.cpp=${OBJ}%.o}
${DIR002}DEPFILES:=${${DIR002}OBJFILES:%.o=%.d}
${DIR002}ASMFILES:=${${DIR002}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR002}BINEXE:=${${DIR002}BINEXE:%=${BIN}/%}
${DIR002}BINCPP:=${${DIR002}BINEXE:${BIN}/%.exe=${DIR002}/%.cpp}
${DIR002}BINOBJ:=${${DIR002}BINCPP:${DIR002}%.cpp=${OBJ}%.o}
${DIR002}BINDEP:=${${DIR002}BINOBJ:%.o=%.d}
${DIR002}BINASM:=${${DIR002}BINOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR002}TSTEXE:=${${DIR002}TSTEXE:%=${BIN}/%}
${DIR002}TSTCPP:=${${DIR002}TSTEXE:${BIN}/%.exe=${DIR002}/%.cpp}
${DIR002}TSTOBJ:=${${DIR002}TSTCPP:${DIR002}%.cpp=${OBJ}%.o}
${DIR002}TSTDEP:=${${DIR002}TSTOBJ:%.o=%.d}
${DIR002}TSTASM:=${${DIR002}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR002}CFILES} \
	${${DIR002}CPPFILES} ${${DIR002}TSTCPP} \
	${${DIR002}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR002}DEPFILES} \
	${${DIR002}TSTDEP} ${${DIR002}BINDEP}
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
.PHONY: ${DIR002}-all ${DIR002}-test \
	${DIR002}-asm ${DIR002}-check \
	${DIR002}-list

#  C++ sources
${OBJ}/%.o: ${DIR002}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR002}QUIET}${CXX} -o $@ -c $< ${${DIR002}CXXFLAGS} ${${DIR002}INCS}
${ASM}/%.s: ${DIR002}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR002}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR002}CXXFLAGS} ${${DIR002}INCS} 

	
TARGET_ALL	:=${TARGET_ALL} ${DIR002}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR002}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR002}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR002}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR002}-list
${DIR002}-all: ${${DIR002}OBJFILES} ${${DIR002}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR002}-test: ${${DIR002}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR002}-asm: ${${DIR002}ASMFILES} ${${DIR002}TSTASM} \
	${${DIR002}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR002}-check: ${${DIR002}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR002}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}$@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR002}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR002}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR002}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR002}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR002}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR002}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
