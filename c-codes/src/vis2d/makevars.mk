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
#  set of files attempt to cut down all the repetitive work that are usually 
#  done manually, i.e., listing target-prerequisites, while preserving almost 
#  the same degree of modularity as that can be achieved through the recursive 
#  make approach.
#  	If you have any suggestions and/or ideas, please let me know.
###############################################################################
#				STEP 1
#DIRECTORY NAME FROM THE ROOT DIRECTORY
#	The ${DIR-vis2d} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-vis2d		:=src/vis2d# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following to be verbose.
${DIR-vis2d}QUIET	:=@
###############################################################################
#				STEP 2
#DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
#
#	Options specified here are used in this directory. By default, all 
#  builds use the same options. When different compiling and/or linking options 
#  need to be assigned to different targets, the programmer needs to list all
#  the speical target-prerequisite dependencies manually.
#  	By default, the local options inherits the corresponding global ones 
#  from the Makefile.vars in the root directory. If one wishes to add any 
#  additional options that are specific to this very directory, add them after 
#  the global ones. For example:
# 	 	${DIR-vis2d}CFLAGS	:=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-vis2d}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
${DIR-vis2d}CFLAGS 	:=${CFLAGS} #-vec-report=1
${DIR-vis2d}CXXFLAGS	:=${CXXFLAGS}
${DIR-vis2d}INCS		:=${INCS}
###############################################################################
#				STEP 3
#DIRECTORY-SPECIFIC SOURCE FILES
#
#  	Remember to add the ${DIR-vis2d}/ to whatever source files(s) you wish to add.
#  The dependencies are automatically resolved by the dependency files (*.d 
#  files) generated by the compiler through the -MD -MP and/or similar options.
#  Note that:
#  	a) The GNU make does NOT support recursive include directive. So we must
#  NOT include any *.d file in this file. All the *.d files are included in the
#  Makefile in the root directory AFTER all the makevars.mk's are include.
#  	b) So far, only supports C/C++ files. Maybe Fortran support is needed 
#  	c) Please refrain from using $(wildcard) command. It only helps when not
#  really nessaccery and complicates the situation when we have forgotten about
#  it. Yes, be explicit. Manually list all the source files here.
#  	d) Dependencies are readily dealt with by the setup of the Makefile. One
#  Please, you NEVER need to specify the dependency for any .o file. Just list
#  the source files.
${DIR-vis2d}CFILES	:=	${DIR-vis2d}/precalc.c	\
				${DIR-vis2d}/geo_utils.c
${DIR-vis2d}CPPFILES	:=	
################## DO NOT MODIFY ################
${DIR-vis2d}OBJFILES	:=	${${DIR-vis2d}CPPFILES:${DIR-vis2d}%.cpp=${BUILD}%.o}	\
				${${DIR-vis2d}CFILES:${DIR-vis2d}%.c=${BUILD}%.o}
${DIR-vis2d}DEPFILES	:=	${${DIR-vis2d}OBJFILES:%.o=%.d}
${DIR-vis2d}ASMFILES	:=	${${DIR-vis2d}OBJFILES:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################ 
###############################################################################
#				STEP 4
#DIRECTORY-SPECIFIC BINARY FILES
#
#	Executables listed in ${DIR}BINFILES are considered the final output of
#  this project. All of them should be built into ${BIN} instead of ${BUILD}
#  for all other executables. 
#  	So far, only supports CPP source files. 
${DIR-vis2d}BIN	:=
#${BIN}/msh_to_data.exe: ${BUILD}/msh_to_data.o ${BUILD}/precalc.o
################## DO NOT MODIFY ################
${DIR-vis2d}BINCPP	:=		${${DIR-vis2d}BIN:%=${DIR-vis2d}/%.cpp}
${DIR-vis2d}BINOBJ	:=		${${DIR-vis2d}BINCPP:${DIR-vis2d}%.cpp=${BUILD}%.o}
${DIR-vis2d}BINDEP	:=		${${DIR-vis2d}BINOBJ:%.o=%.d}
${DIR-vis2d}BINEXE	:=		${${DIR-vis2d}BINOBJ:${BUILD}%.o=${BIN}%.exe}
${DIR-vis2d}BINASM	:=		${${DIR-vis2d}BINOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
###############################################################################
#				STEP 5
#DIRECTORY-SPECIFIC TEST FILES

#	Speicify all the test files. All test files must be CPP files. But when
#  listing the them in ${DIR-vis2d}TST, do NOT write the .cpp extension. For example:
#  if one wishes to add test-mytest.cpp, he should write:
#
#  		${DIR-vis2d}TST:=test-mytest
#
#	Then list all the build rules right afterwards. For example:
#
#		${BUILD}/test_mytest.exe:	${BUILD}/test_mytest.o		\
#						${BUILD}/any_other_files.o
${DIR_precalc}TST	:=		test_precalc test_geo_utils
${BUILD}/test_precalc.exe: 	${BUILD}/test_precalc.o ${BUILD}/precalc.o \
		${BUILD}/utils.o
${BUILD}/test_geo_utils.exe: 	${BUILD}/test_geo_utils.o ${BUILD}/geo_utils.o \
		${BUILD}/utils.o
################## DO NOT MODIFY ################
${DIR_precalc}TSTCPP	:=		${${DIR_precalc}TST:%=${DIR_precalc}/%.cpp}
${DIR-vis2d}TSTOBJ	:=		${${DIR-vis2d}TSTCPP:${DIR-vis2d}%.cpp=${BUILD}%.o}
${DIR-vis2d}TSTDEP	:=		${${DIR-vis2d}TSTOBJ:%.o=%.d}
${DIR-vis2d}TSTEXE	:=		${${DIR-vis2d}TSTOBJ:%.o=%.exe}
${DIR-vis2d}TSTASM	:=		${${DIR-vis2d}TSTOBJ:${BUILD}%.o=${ASM}%.s}
SRCFILES	:=		${SRCFILES} ${${DIR-vis2d}CFILES} ${${DIR-vis2d}CPPFILES} ${${DIR-vis2d}TSTCPP} ${${DIR-vis2d}BINCPP}
DEPFILES	:=		${DEPFILES} ${${DIR-vis2d}DEPFILES} ${${DIR-vis2d}TSTDEP} ${${DIR-vis2d}BINDEP}
################## DO NOT MODIFY ################
###############################################################################
#				STEP 6
#	Write whatever special dependencies that do not fit into any pattern
#  rules here:






###############################################################################
###############################################################################
###############################################################################
#	Congratulations! You have completed everything you need to do to build
#  this directory. You do not need to modify this file unless some C and/or
#  CPP source files are added, removed, or renamed. In that case, just go over
#  STEP 1-6 again and it is done!
###############################################################################
###############################################################################
###############################################################################
#				WANRING
#  Please do not modify the following part if you are not 100% sure about it.
#  The design of this file is that one only needs to modify the part above.
#  If you find you need to modify any of the parts below, be very careful!
#				WANRING
###############################################################################
###############################################################################
############################################################################### 
#DIRECTORY-SPECIFIC PATTERN RULES
#
#	C++ linkage at the top level is done in the Makefile.vars in the root 
#  directory. Any special build rules for libraries and executables should be
#  secified in STEP 6 in this makevars.mk file.

#  C++ sources
${BUILD}/%.o: ${DIR-vis2d}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-vis2d}QUIET}${CXX} -o $@ -c $< ${${DIR-vis2d}CXXFLAGS} ${${DIR-vis2d}INCS}
${ASM}/%.s: ${DIR-vis2d}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-vis2d}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-vis2d}CXXFLAGS} ${${DIR-vis2d}INCS} 
#  C sources
${BUILD}/%.o: ${DIR-vis2d}/%.c
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-vis2d}QUIET}${CC} -o $@ -c $< ${${DIR-vis2d}CFLAGS} ${${DIR-vis2d}INCS}
${ASM}/%.s: ${DIR-vis2d}/%.c
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-vis2d}QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR-vis2d}CFLAGS} ${${DIR-vis2d}INCS} 
#DIR-vis2dECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR-vis2d}-all ${DIR-vis2d}-test ${DIR-vis2d}-asm ${DIR-vis2d}-list
TARGET_ALL	:=${TARGET_ALL} ${DIR-vis2d}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-vis2d}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-vis2d}-asm
TARGET_LIST	:=${TARGET_LIST} ${DIR-vis2d}-list
${DIR-vis2d}-all: ${${DIR-vis2d}OBJFILES} ${${DIR-vis2d}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-vis2d}-test: ${${DIR-vis2d}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-vis2d}-asm: ${${DIR-vis2d}ASMFILES} ${${DIR-vis2d}TSTASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-vis2d}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@echo "${BROWN}CFILES${NONE}":${${DIR-vis2d}CFILES}
	@echo "${BROWN}CPPFILES${NONE}":${${DIR-vis2d}CPPFILES}
	@echo "${BROWN}OBJFILES${NONE}":${${DIR-vis2d}OBJFILES}
	@echo "${BROWN}DEPFILES${NONE}":${${DIR-vis2d}DEPFILES}
	@echo "${BROWN}ASMFILES${NONE}":${${DIR-vis2d}ASMFILES}
	@echo "${BROWN}TSTCPP${NONE}":${${DIR-vis2d}TSTCPP}
	@echo "${BROWN}TSTOBJ${NONE}":${${DIR-vis2d}TSTOBJ}
	@echo "${BROWN}TSTDEP${NONE}":${${DIR-vis2d}TSTDEP}
	@echo "${BROWN}TSTEXE${NONE}":${${DIR-vis2d}TSTEXE}
	@echo "${BROWN}TSTASM${NONE}":${${DIR-vis2d}TSTASM} 
	@echo "${BROWN}BINCPP${NONE}":${${DIR-vis2d}BINCPP}
	@echo "${BROWN}BINOBJ${NONE}":${${DIR-vis2d}BINOBJ}
	@echo "${BROWN}BINDEP${NONE}":${${DIR-vis2d}BINDEP}
	@echo "${BROWN}BINEXE${NONE}":${${DIR-vis2d}BINEXE}
	@echo "${BROWN}BINASM${NONE}":${${DIR-vis2d}BINASM} 
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}END $@${NONE}"\#\#\#\#\#\#\#\#
