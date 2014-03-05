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
DIR006:=${SRC}/quadrules# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR006}QUIET:=@
###############################################################################
#				STEP 2
#	DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
${DIR006}CXXFLAGS:=${CXXFLAGS}
${DIR006}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR006}CPPFILES:=arcsinh.cpp dunavant.cpp gauss.cpp wandzura.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR006}BINEXE:=
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR006}TSTEXE:=
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
${DIR006}CPPFILES:=${${DIR006}CPPFILES:%=${DIR006}/%}
${DIR006}OBJFILES:=${${DIR006}CPPFILES:${DIR006}%.cpp=${OBJ}%.o}
${DIR006}DEPFILES:=${${DIR006}OBJFILES:%.o=%.d}
${DIR006}ASMFILES:=${${DIR006}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR006}BINEXE:=${${DIR006}BINEXE:%=${BIN}/%}
${DIR006}BINCPP:=${${DIR006}BINEXE:${BIN}/%.exe=${DIR006}/%.cpp}
${DIR006}BINOBJ:=${${DIR006}BINCPP:${DIR006}%.cpp=${OBJ}%.o}
${DIR006}BINDEP:=${${DIR006}BINOBJ:%.o=%.d}
${DIR006}BINASM:=${${DIR006}BINOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR006}TSTEXE:=${${DIR006}TSTEXE:%=${BIN}/%}
${DIR006}TSTCPP:=${${DIR006}TSTEXE:${BIN}/%.exe=${DIR006}/%.cpp}
${DIR006}TSTOBJ:=${${DIR006}TSTCPP:${DIR006}%.cpp=${OBJ}%.o}
${DIR006}TSTDEP:=${${DIR006}TSTOBJ:%.o=%.d}
${DIR006}TSTASM:=${${DIR006}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR006}CFILES} \
	${${DIR006}CPPFILES} ${${DIR006}TSTCPP} \
	${${DIR006}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR006}DEPFILES} \
	${${DIR006}TSTDEP} ${${DIR006}BINDEP}
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
.PHONY: ${DIR006}-all ${DIR006}-test \
	${DIR006}-asm ${DIR006}-check \
	${DIR006}-list

#  C++ sources
${OBJ}/%.o: ${DIR006}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR006}QUIET}${CXX} -o $@ -c $< ${${DIR006}CXXFLAGS} ${${DIR006}INCS}
${ASM}/%.s: ${DIR006}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR006}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR006}CXXFLAGS} ${${DIR006}INCS} 

	
TARGET_ALL	:=${TARGET_ALL} ${DIR006}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR006}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR006}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR006}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR006}-list
${DIR006}-all: ${${DIR006}OBJFILES} ${${DIR006}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR006}-test: ${${DIR006}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR006}-asm: ${${DIR006}ASMFILES} ${${DIR006}TSTASM} \
	${${DIR006}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR006}-check: ${${DIR006}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR006}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}$@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR006}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR006}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR006}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR006}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR006}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR006}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
