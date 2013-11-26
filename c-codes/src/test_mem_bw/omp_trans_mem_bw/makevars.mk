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
#	The ${DIR-omp_trans_membw} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-test_mem_bw/-omp_trans_mem_bw		:=src/test_mem_bw/omp_trans_mem_bw# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following to be verbose.
${DIR-test_mem_bw/-omp_trans_mem_bw}QUIET	:=@
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
# 	 	${DIR-test_mem_bw/-omp_trans_mem_bw}CFLAGS	:=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-test_mem_bw/-omp_trans_mem_bw}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
${DIR-test_mem_bw/-omp_trans_mem_bw}CFLAGS 	:=${CFLAGS}
${DIR-test_mem_bw/-omp_trans_mem_bw}CXXFLAGS	:=${CXXFLAGS} #-vec-report=1
${DIR-test_mem_bw/-omp_trans_mem_bw}INCS		:=${INCS}
###############################################################################
#				STEP 3
#DIRECTORY-SPECIFIC SOURCE FILES
#
#  	Remember to add the ${DIR-test_mem_bw/-omp_trans_mem_bw}/ to whatever source files(s) you wish to add.
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
${DIR-test_mem_bw/-omp_trans_mem_bw}CFILES	:=	${DIR-test_mem_bw/-omp_trans_mem_bw}/trans.c
${DIR-test_mem_bw/-omp_trans_mem_bw}CPPFILES	:=	
################## DO NOT MODIFY ################
${DIR-test_mem_bw/-omp_trans_mem_bw}OBJFILES	:=	${${DIR-test_mem_bw/-omp_trans_mem_bw}CPPFILES:${DIR-test_mem_bw/-omp_trans_mem_bw}%.cpp=${BUILD}%.o}	\
				${${DIR-test_mem_bw/-omp_trans_mem_bw}CFILES:${DIR-test_mem_bw/-omp_trans_mem_bw}%.c=${BUILD}%.o}
${DIR-test_mem_bw/-omp_trans_mem_bw}DEPFILES	:=	${${DIR-test_mem_bw/-omp_trans_mem_bw}OBJFILES:%.o=%.d}
${DIR-test_mem_bw/-omp_trans_mem_bw}ASMFILES	:=	${${DIR-test_mem_bw/-omp_trans_mem_bw}OBJFILES:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################ 
###############################################################################
#				STEP 4
#DIRECTORY-SPECIFIC BINARY FILES
#
#	Executables listed in ${DIR}BINFILES are considered the final output of
#  this project. All of them should be built into ${BIN} instead of ${BUILD}
#  for all other executables. 
#  	So far, only supports CPP source files. 
${DIR-test_mem_bw/-omp_trans_mem_bw}BIN	:=	
################## DO NOT MODIFY ################
${DIR-test_mem_bw/-omp_trans_mem_bw}BINCPP	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}BIN:%=${DIR-test_mem_bw/-omp_trans_mem_bw}/%.cpp}
${DIR-test_mem_bw/-omp_trans_mem_bw}BINOBJ	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}BINCPP:${DIR-test_mem_bw/-omp_trans_mem_bw}%.cpp=${BUILD}%.o}
${DIR-test_mem_bw/-omp_trans_mem_bw}BINDEP	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}BINOBJ:%.o=%.d}
${DIR-test_mem_bw/-omp_trans_mem_bw}BINEXE	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}BINOBJ:${BUILD}%.o=${BIN}%.exe}
${DIR-test_mem_bw/-omp_trans_mem_bw}BINASM	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}BINOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
###############################################################################
#				STEP 5
#DIRECTORY-SPECIFIC TEST FILES

#	Speicify all the test files. All test files must be CPP files. But when
#  listing the them in ${DIR-test_mem_bw/-omp_trans_mem_bw}TST, do NOT write the .cpp extension. For example:
#  if one wishes to add test-mytest.cpp, he should write:
#
#  		${DIR-test_mem_bw/-omp_trans_mem_bw}TST:=test-mytest
#
#	Then list all the build rules right afterwards. For example:
#
#		${BUILD}/test-mytest.exe:	${BUILD}/test-mytest.o		\
#						${BUILD}/any-other-files.o
${DIR-test_mem_bw/-omp_trans_mem_bw}TST	:=		test_trans time_trans
${BUILD}/test_trans.exe: 	${BUILD}/test_trans.o ${BUILD}/trans.o ${BUILD}/utils.o
${BUILD}/time_trans.exe: 	${BUILD}/time_trans.o ${BUILD}/trans.o ${BUILD}/utils.o
################## DO NOT MODIFY ################
${DIR-test_mem_bw/-omp_trans_mem_bw}TSTCPP	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}TST:%=${DIR-test_mem_bw/-omp_trans_mem_bw}/%.cpp}
${DIR-test_mem_bw/-omp_trans_mem_bw}TSTOBJ	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTCPP:${DIR-test_mem_bw/-omp_trans_mem_bw}%.cpp=${BUILD}%.o}
${DIR-test_mem_bw/-omp_trans_mem_bw}TSTDEP	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTOBJ:%.o=%.d}
${DIR-test_mem_bw/-omp_trans_mem_bw}TSTEXE	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTOBJ:%.o=%.exe}
${DIR-test_mem_bw/-omp_trans_mem_bw}TSTASM	:=		${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTOBJ:${BUILD}%.o=${ASM}%.s}
SRCFILES	:=		${SRCFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}CFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}CPPFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTCPP} ${${DIR-test_mem_bw/-omp_trans_mem_bw}BINCPP}
DEPFILES	:=		${DEPFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}DEPFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTDEP} ${${DIR-test_mem_bw/-omp_trans_mem_bw}BINDEP}
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
${BUILD}/%.o: ${DIR-test_mem_bw/-omp_trans_mem_bw}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-test_mem_bw/-omp_trans_mem_bw}QUIET}${CXX} -o $@ -c $< ${${DIR-test_mem_bw/-omp_trans_mem_bw}CXXFLAGS} ${${DIR-test_mem_bw/-omp_trans_mem_bw}INCS}
${ASM}/%.s: ${DIR-test_mem_bw/-omp_trans_mem_bw}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-test_mem_bw/-omp_trans_mem_bw}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-test_mem_bw/-omp_trans_mem_bw}CXXFLAGS} ${${DIR-test_mem_bw/-omp_trans_mem_bw}INCS} 
#  C sources
${BUILD}/%.o: ${DIR-test_mem_bw/-omp_trans_mem_bw}/%.c
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-test_mem_bw/-omp_trans_mem_bw}QUIET}${CC} -o $@ -c $< ${${DIR-test_mem_bw/-omp_trans_mem_bw}CFLAGS} ${${DIR-test_mem_bw/-omp_trans_mem_bw}INCS}
${ASM}/%.s: ${DIR-test_mem_bw/-omp_trans_mem_bw}/%.c
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-test_mem_bw/-omp_trans_mem_bw}QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR-test_mem_bw/-omp_trans_mem_bw}CFLAGS} ${${DIR-test_mem_bw/-omp_trans_mem_bw}INCS} 
#DIR-test_mem_bw/-omp_trans_mem_bwECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR-test_mem_bw/-omp_trans_mem_bw}-all ${DIR-test_mem_bw/-omp_trans_mem_bw}-test ${DIR-test_mem_bw/-omp_trans_mem_bw}-asm ${DIR-test_mem_bw/-omp_trans_mem_bw}-list
TARGET_ALL	:=${TARGET_ALL} ${DIR-test_mem_bw/-omp_trans_mem_bw}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-test_mem_bw/-omp_trans_mem_bw}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-test_mem_bw/-omp_trans_mem_bw}-asm
TARGET_LIST	:=${TARGET_LIST} ${DIR-test_mem_bw/-omp_trans_mem_bw}-list
${DIR-test_mem_bw/-omp_trans_mem_bw}-all: ${${DIR-test_mem_bw/-omp_trans_mem_bw}OBJFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-test_mem_bw/-omp_trans_mem_bw}-test: ${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-test_mem_bw/-omp_trans_mem_bw}-asm: ${${DIR-test_mem_bw/-omp_trans_mem_bw}ASMFILES} ${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-test_mem_bw/-omp_trans_mem_bw}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@echo "${BROWN}CFILES${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}CFILES}
	@echo "${BROWN}CPPFILES${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}CPPFILES}
	@echo "${BROWN}OBJFILES${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}OBJFILES}
	@echo "${BROWN}DEPFILES${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}DEPFILES}
	@echo "${BROWN}ASMFILES${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}ASMFILES}
	@echo "${BROWN}TSTCPP${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTCPP}
	@echo "${BROWN}TSTOBJ${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTOBJ}
	@echo "${BROWN}TSTDEP${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTDEP}
	@echo "${BROWN}TSTEXE${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTEXE}
	@echo "${BROWN}TSTASM${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}TSTASM} 
	@echo "${BROWN}BINCPP${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}BINCPP}
	@echo "${BROWN}BINOBJ${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}BINOBJ}
	@echo "${BROWN}BINDEP${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}BINDEP}
	@echo "${BROWN}BINEXE${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}BINEXE}
	@echo "${BROWN}BINASM${NONE}":${${DIR-test_mem_bw/-omp_trans_mem_bw}BINASM} 
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}END $@${NONE}"\#\#\#\#\#\#\#\#