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
#
#  The ${DIR-v1} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-v1:=src/v1# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR-v1}QUIET:=@
###############################################################################
#				STEP 2
#	DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
#
#  Options specified here are used in this directory. By default, all builds 
#  use the same options. When different compiling and/or linking options 
#  need to be assigned to different targets, the programmer needs to list all
#  the speical target-prerequisite dependencies manually.
#  	By default, the local options inherits the corresponding global ones 
#  from the Makefile in the root directory. If one wishes to add any 
#  additional options that are specific to this very directory, add them after 
#  the global ones. For example:
# 	 	${DIR-v1}CFLAGS :=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-v1}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
#  	Note that one _cannot_ drop any of the global compiling and linking
#  options. So only specify options that are definitely needed by the entire
#  project at the top level Makefile.
${DIR-v1}CXXFLAGS:=${CXXFLAGS}
${DIR-v1}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR-v1}CPPFILES:=Solver_v1.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR-v1}BINEXE:=
${DIR-v1}LIB:=
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR-v1}TSTEXE:=test_v1.exe

${BIN}/test_Solver_v1.exe: ${OBJ}/test_Solver_v1.o \
	${OBJ}/Solver_v1.o ${OBJ}/file_io.o \
	${LIB}/libutils.a ${LIB}/libutils.so \
	${LIB}/libQuadratureRules.a ${LIB}/libQuadratureRules.so

${OUTPUT}/test_Solver_v1.txt: ${BIN}/test_Solver_v1.exe \
	${EXAMPLE}/square162.binary.info

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
${DIR-v1}CPPFILES:=${${DIR-v1}CPPFILES:%=${DIR-v1}/%}
${DIR-v1}OBJFILES:=${${DIR-v1}CPPFILES:${DIR-v1}%.cpp=${OBJ}%.o}
${DIR-v1}DEPFILES:=${${DIR-v1}OBJFILES:%.o=%.d}
${DIR-v1}ASMFILES:=${${DIR-v1}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-v1}BINEXE:=${${DIR-v1}BINEXE:%=${BIN}/%}
${DIR-v1}BINCPP:=${${DIR-v1}BINEXE:${BIN}/%.exe=${DIR-v1}/%.cpp}
${DIR-v1}BINOBJ:=${${DIR-v1}BINCPP:${DIR-v1}%.cpp=${OBJ}%.o}
${DIR-v1}BINDEP:=${${DIR-v1}BINOBJ:%.o=%.d}
${DIR-v1}BINASM:=${${DIR-v1}BINOBJ:${OBJ}%.o=${ASM}%.s}
${DIR-v1}DYNLIB:=${${DIR-v1}LIB:%=${LIB}/lib%.so}
${DIR-v1}STCLIB:=${${DIR-v1}LIB:%=${LIB}/lib%.a}
################## DO NOT MODIFY ################
${DIR-v1}TSTEXE:=${${DIR-v1}TSTEXE:%=${BIN}/%}
${DIR-v1}TSTCPP:=${${DIR-v1}TSTEXE:${BIN}/%.exe=${DIR-v1}/%.cpp}
${DIR-v1}TSTOBJ:=${${DIR-v1}TSTCPP:${DIR-v1}%.cpp=${OBJ}%.o}
${DIR-v1}TSTDEP:=${${DIR-v1}TSTOBJ:%.o=%.d}
${DIR-v1}TSTASM:=${${DIR-v1}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR-v1}CFILES} \
	${${DIR-v1}CPPFILES} ${${DIR-v1}TSTCPP} \
	${${DIR-v1}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR-v1}DEPFILES} \
	${${DIR-v1}TSTDEP} ${${DIR-v1}BINDEP}
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

#  C++ sources
${OBJ}/%.o: ${DIR-v1}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-v1}QUIET}${CXX} -o $@ -c $< ${${DIR-v1}CXXFLAGS} ${${DIR-v1}INCS}
${ASM}/%.s: ${DIR-v1}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-v1}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-v1}CXXFLAGS} ${${DIR-v1}INCS} 
#DIR-v1ECTORY-SPECIFIC PHONY TARGETS
#.PHONY: ${DIR-v1}-all ${DIR-v1}-test \
	#${DIR-v1}-asm ${DIR-v1}-check \
	#${DIR-v1}-list
	
TARGET_ALL	:=${TARGET_ALL} ${DIR-v1}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-v1}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-v1}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR-v1}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR-v1}-list
${DIR-v1}-all: ${${DIR-v1}OBJFILES} ${${DIR-v1}BINEXE}	\
	${${DIR-v1}DYNLIB} ${${DIR-v1}STCLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-v1}-test: ${${DIR-v1}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-v1}-asm: ${${DIR-v1}ASMFILES} ${${DIR-v1}TSTASM} \
	${${DIR-v1}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-v1}-check: ${${DIR-v1}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR-v1}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR-v1}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-v1}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR-v1}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-v1}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		DYNLIB STCLIB							\
		,							\
		if [ ! -z "${${DIR-v1}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-v1}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${MAGENTA}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR-v1}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-v1}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
	@#@$(foreach dir, 						\
		#ASMFILES TSTASM BINASM					\
		#,							\
		#if [ ! -z "${${DIR-v1}${dir}}" ]; then 		\
			#echo "${BROWN}${dir}${NONE}\t\c";		\
			#$(foreach file,${${DIR-v1}${dir}},		\
			    #if [ -f ${file} ]; then echo		\
				#"${CYAN}${file}${NONE}\c";		\
			    #else echo "${GREY}${file}${NONE}\c";	\
			    #fi;)					\
			#echo;						\
		#fi;)
