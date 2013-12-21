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
#  The ${DIR-utils} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-utils:=src/utils# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR-utils}QUIET:=@
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
# 	 	${DIR-utils}CFLAGS :=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-utils}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
#  	Note that one _cannot_ drop any of the global compiling and linking
#  options. So only specify options that are definitely needed by the entire
#  project at the top level Makefile.
${DIR-utils}CXXFLAGS:=${CXXFLAGS}
${DIR-utils}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR-utils}CPPFILES:=utils.cpp StatVector.cpp Table.cpp TimeStamp.cpp \
	DunavantRule.cpp GaussRule.cpp LynessRule.cpp \
	WandzuraRule.cpp int_trig.cpp QuadratureRules.cpp \
	toeplitz.cpp file_io.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR-utils}BINEXE:=dump_msh.exe
${DIR-utils}LIB:=utils QuadratureRules

${BIN}/dump_msh.exe: ${OBJ}/dump_msh.o \
	${OBJ}/file_io.o \
	${LIB}/libutils.a ${LIB}/libutils.so 

${LIB}/libutils.so ${LIB}/libutils.a: ${OBJ}/utils.o \
	${OBJ}/StatVector.o ${OBJ}/Table.o ${OBJ}/TimeStamp.o \

${LIB}/libQuadratureRules.so ${LIB}/libQuadratureRules.a: \
	${OBJ}/QuadratureRules.o \
	${OBJ}/GaussRule.o ${OBJ}/DunavantRule.o \
	${OBJ}/WandzuraRule.o ${OBJ}/LynessRule.o
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR-utils}TSTEXE:=test_utils.exe test_QuadratureRules.exe \
	test_int_trig.exe test_toeplitz.exe test_mkl_solvers.exe \
	test_file_io.exe

${BIN}/test_utils.exe: ${OBJ}/test_utils.o \
	${LIB}/libutils.a ${LIB}/libutils.so 

${BIN}/test_int_trig.exe: ${OBJ}/test_int_trig.o \
	${OBJ}/int_trig.o \
	${LIB}/libutils.a ${LIB}/libutils.so \
	${LIB}/libQuadratureRules.a ${LIB}/libQuadratureRules.so

${BIN}/test_QuadratureRules.exe: ${OBJ}/test_QuadratureRules.o \
	${LIB}/libutils.a ${LIB}/libutils.so \
	${LIB}/libQuadratureRules.a ${LIB}/libQuadratureRules.so

${BIN}/test_toeplitz.exe: ${OBJ}/test_toeplitz.o \
	${OBJ}/toeplitz.o \
	${LIB}/libutils.a ${LIB}/libutils.so

${BIN}/test_mkl_solvers.exe: ${OBJ}/test_mkl_solvers.o \
	${LIB}/libutils.a ${LIB}/libutils.so

${BIN}/test_file_io.exe: ${OBJ}/test_file_io.o \
	${OBJ}/file_io.o \
	${LIB}/libutils.a ${LIB}/libutils.so \
	${EXAMPLE}/square162.ascii.info \
	${EXAMPLE}/square162.binary.info

${EXAMPLE}/square162.ascii.info : ${BIN}/dump_msh.exe \
	${EXAMPLE}/square162.msh
	@echo "Dumping ${EXAMPLE}/square162.msh to ASCII..."
	@./$< 1 ${EXAMPLE}/square162.msh ${EXAMPLE}/square162.ascii >> log

${EXAMPLE}/square162.binary.info : ${BIN}/dump_msh.exe \
	${EXAMPLE}/square162.msh
	@echo "Dumping ${EXAMPLE}/square162.msh to BINARY..."
	@./$< 2 ${EXAMPLE}/square162.msh ${EXAMPLE}/square162.binary >> log

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
${DIR-utils}CPPFILES:=${${DIR-utils}CPPFILES:%=${DIR-utils}/%}
${DIR-utils}OBJFILES:=${${DIR-utils}CPPFILES:${DIR-utils}%.cpp=${OBJ}%.o}
${DIR-utils}DEPFILES:=${${DIR-utils}OBJFILES:%.o=%.d}
${DIR-utils}ASMFILES:=${${DIR-utils}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-utils}BINEXE:=${${DIR-utils}BINEXE:%=${BIN}/%}
${DIR-utils}BINCPP:=${${DIR-utils}BINEXE:${BIN}/%.exe=${DIR-utils}/%.cpp}
${DIR-utils}BINOBJ:=${${DIR-utils}BINCPP:${DIR-utils}%.cpp=${OBJ}%.o}
${DIR-utils}BINDEP:=${${DIR-utils}BINOBJ:%.o=%.d}
${DIR-utils}BINASM:=${${DIR-utils}BINOBJ:${OBJ}%.o=${ASM}%.s}
${DIR-utils}DYNLIB:=${${DIR-utils}LIB:%=${LIB}/lib%.so}
${DIR-utils}STCLIB:=${${DIR-utils}LIB:%=${LIB}/lib%.a}
################## DO NOT MODIFY ################
${DIR-utils}TSTEXE:=${${DIR-utils}TSTEXE:%=${BIN}/%}
${DIR-utils}TSTCPP:=${${DIR-utils}TSTEXE:${BIN}/%.exe=${DIR-utils}/%.cpp}
${DIR-utils}TSTOBJ:=${${DIR-utils}TSTCPP:${DIR-utils}%.cpp=${OBJ}%.o}
${DIR-utils}TSTDEP:=${${DIR-utils}TSTOBJ:%.o=%.d}
${DIR-utils}TSTASM:=${${DIR-utils}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR-utils}CFILES} \
	${${DIR-utils}CPPFILES} ${${DIR-utils}TSTCPP} \
	${${DIR-utils}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR-utils}DEPFILES} \
	${${DIR-utils}TSTDEP} ${${DIR-utils}BINDEP}
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
${OBJ}/%.o: ${DIR-utils}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-utils}QUIET}${CXX} -o $@ -c $< ${${DIR-utils}CXXFLAGS} ${${DIR-utils}INCS}
${ASM}/%.s: ${DIR-utils}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-utils}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-utils}CXXFLAGS} ${${DIR-utils}INCS} 
#DIR-utilsECTORY-SPECIFIC PHONY TARGETS
#.PHONY: ${DIR-utils}-all ${DIR-utils}-test \
	#${DIR-utils}-asm ${DIR-utils}-check \
	#${DIR-utils}-list
	
TARGET_ALL	:=${TARGET_ALL} ${DIR-utils}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-utils}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-utils}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR-utils}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR-utils}-list
${DIR-utils}-all: ${${DIR-utils}OBJFILES} ${${DIR-utils}BINEXE}	\
	${${DIR-utils}DYNLIB} ${${DIR-utils}STCLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-utils}-test: ${${DIR-utils}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-utils}-asm: ${${DIR-utils}ASMFILES} ${${DIR-utils}TSTASM} \
	${${DIR-utils}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-utils}-check: ${${DIR-utils}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR-utils}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR-utils}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-utils}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR-utils}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-utils}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		DYNLIB STCLIB							\
		,							\
		if [ ! -z "${${DIR-utils}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-utils}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${MAGENTA}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR-utils}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-utils}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
	@#@$(foreach dir, 						\
		#ASMFILES TSTASM BINASM					\
		#,							\
		#if [ ! -z "${${DIR-utils}${dir}}" ]; then 		\
			#echo "${BROWN}${dir}${NONE}\t\c";		\
			#$(foreach file,${${DIR-utils}${dir}},		\
			    #if [ -f ${file} ]; then echo		\
				#"${CYAN}${file}${NONE}\c";		\
			    #else echo "${GREY}${file}${NONE}\c";	\
			    #fi;)					\
			#echo;						\
		#fi;)
