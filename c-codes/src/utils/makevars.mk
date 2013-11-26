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
#	The ${DIR-utils} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-utils		:=src/utils# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following to be verbose.
${DIR-utils}QUIET	:=@
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
# 	 	${DIR-utils}CFLAGS	:=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-utils}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
${DIR-utils}CFLAGS 	:=${CFLAGS}
${DIR-utils}CXXFLAGS	:=${CXXFLAGS}
${DIR-utils}INCS	:=${INCS}
###############################################################################
#				STEP 3
#DIRECTORY-SPECIFIC SOURCE FILES
#
#  	Remember to add the ${DIR-utils}/ to whatever source files(s) you wish to add.
#  The dependencies are automatically resolved by the dependency files (*.d 
#  files) generated by the compiler through the -MD -MP and/or similar options.
#  Note that:
#  	a) The GNU make does NOT support recursive include directive. So we must
#  NOT include any *.d file in this file. All the *.d files are included in the
#  Makefile in the root directory AFTER all the makevars.mk's are include.
#  	b) So far, only supports C/C++ files. Maybe Fortran support is needed TODO
#  	c) Please refrain from using $(wildcard) command. It only helps when not
#  really nessaccery and complicates the situation when we have forgotten about
#  it. Yes, be explicit. Manually list all the source files here.
#  	d) Dependencies are readily dealt with by the setup of the Makefile. One
#  Please, you NEVER need to specify the dependency for any .o file. Just list
#  the source files.
${DIR-utils}CFILES	:=	${DIR-utils}/c_utils.c 
${DIR-utils}CPPFILES	:=	${DIR-utils}/cpp_utils.cpp \
	${DIR-utils}/Table.cpp ${DIR-utils}/StatVector.cpp \
	${DIR-utils}/TimeStamp.cpp
#	Specify the libraries one wishes to build, and the build rules for them.
#  Note that these libraries are cleaned when one types "make clean".
${DIR-utils}LIBFILES	:=	
################## DO NOT MODIFY ################
${DIR-utils}OBJFILES	:=	${${DIR-utils}CPPFILES:${DIR-utils}%.cpp=${BUILD}%.o}	\
				${${DIR-utils}CFILES:${DIR-utils}%.c=${BUILD}%.o}
${DIR-utils}DEPFILES	:=	${${DIR-utils}OBJFILES:%.o=%.d}
${DIR-utils}ASMFILES	:=	${${DIR-utils}OBJFILES:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################ 
###############################################################################
#				STEP 4
#DIRECTORY-SPECIFIC BINARIES
#
#	Executables listed in ${DIR}BINFILES are considered the final output of
#  this project. All of them should be built into ${BIN} instead of ${BUILD}
#  for all other executables.
#  	Also, specify the dependencies for each binaries.
${DIR-utils}BIN	:=		
#	Specify the libraries one wishes to build, and the build rules for them.
#  Note that these libraries are considered part of the final output.
${DIR-utils}BINLIB	:=	${BIN}/cpp_utils.dylib ${BIN}/c_utils.dylib
${BIN}/cpp_utils.dylib: ${BUILD}/cpp_utils.o \
	${BUILD}/Table.o ${BUILD}/StatVector.o ${BUILD}/TimeStamp.o
${BIN}/c_utils.dylib: ${BUILD}/c_utils.o 
################## DO NOT MODIFY ################
${DIR-utils}BINCPP	:=		${${DIR-utils}BIN:%=${DIR-utils}/%.cpp}
${DIR-utils}BINOBJ	:=		${${DIR-utils}BINCPP:${DIR-utils}%.cpp=${BUILD}%.o}
${DIR-utils}BINDEP	:=		${${DIR-utils}BINOBJ:%.o=%.d}
${DIR-utils}BINEXE	:=		${${DIR-utils}BINOBJ:${BUILD}%.o=${BIN}%.exe}
${DIR-utils}BINASM	:=		${${DIR-utils}BINOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
###############################################################################
#				STEP 5
#DIRECTORY-SPECIFIC TEST FILES

#	Speicify all the test files. All test files must be CPP files. But when
#  listing the them in ${DIR-utils}TST, do NOT write the .cpp extension. For example:
#  if one wishes to add test_mytest.cpp, he should write:
#
#  		${DIR-utils}TST:=test_mytest
#
#	Then list all the build rules right afterwards. For example:
#
#		${BUILD}/test_mytest.exe:	${BUILD}/test_mytest.o		\
#						${BUILD}/any-other-files.o
${DIR-utils}TST	:=		test_c_utils test_cpp_utils test_geo_utils
${BUILD}/test_c_utils.exe: ${BIN}/c_utils.dylib ${BUILD}/test_c_utils.o
${BUILD}/test_cpp_utils.exe: ${BIN}/cpp_utils.dylib ${BUILD}/test_cpp_utils.o
${BUILD}/test_geo_utils.exe: ${BIN}/cpp_utils.dylib ${BUILD}/test_geo_utils.o \
	${BUILD}/geo_utils.o
################## DO NOT MODIFY ################
${DIR-utils}TSTCPP	:=		${${DIR-utils}TST:%=${DIR-utils}/%.cpp}
${DIR-utils}TSTOBJ	:=		${${DIR-utils}TSTCPP:${DIR-utils}%.cpp=${BUILD}%.o}
${DIR-utils}TSTDEP	:=		${${DIR-utils}TSTOBJ:%.o=%.d}
${DIR-utils}TSTEXE	:=		${${DIR-utils}TSTOBJ:%.o=%.exe}
${DIR-utils}TSTASM	:=		${${DIR-utils}TSTOBJ:${BUILD}%.o=${ASM}%.s}
SRCFILES	:=		${SRCFILES} ${${DIR-utils}CFILES} ${${DIR-utils}CPPFILES} ${${DIR-utils}TSTCPP} ${${DIR-utils}BINCPP}
DEPFILES	:=		${DEPFILES} ${${DIR-utils}DEPFILES} ${${DIR-utils}TSTDEP} ${${DIR-utils}BINDEP}
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
${BUILD}/%.o: ${DIR-utils}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-utils}QUIET}${CXX} -o $@ -c $< ${${DIR-utils}CXXFLAGS} ${${DIR-utils}INCS}
${ASM}/%.s: ${DIR-utils}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-utils}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-utils}CXXFLAGS} ${${DIR-utils}INCS} 
#  C sources
${BUILD}/%.o: ${DIR-utils}/%.c
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-utils}QUIET}${CC} -o $@ -c $< ${${DIR-utils}CFLAGS} ${${DIR-utils}INCS}
${ASM}/%.s: ${DIR-utils}/%.c
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-utils}QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR-utils}CFLAGS} ${${DIR-utils}INCS} 
#DIR-utilsECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR-utils}-all ${DIR-utils}-test ${DIR-utils}-asm ${DIR-utils}-list
TARGET_ALL	:=${TARGET_ALL} ${DIR-utils}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-utils}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-utils}-asm
TARGET_LIST	:=${TARGET_LIST} ${DIR-utils}-list
${DIR-utils}-all: ${${DIR-utils}OBJFILES} ${${DIR-utils}BINEXE}		\
	${${DIR-utils}LIBFILES} ${${DIR-utils}BINLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-utils}-test: ${${DIR-utils}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-utils}-asm: ${${DIR-utils}ASMFILES} ${${DIR-utils}TSTASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
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
	@$(foreach dir, 						\
		DEPFILES TSTDEP BINDEP					\
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
		LIBFILES BINLIB						\
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
	@#echo \#\#\#\#\#\#\#\#"${B_BROWN}END $@${NONE}"\#\#\#\#\#\#\#\# 
