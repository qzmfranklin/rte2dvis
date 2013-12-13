################################################################################
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
################################################################################
#  Welcome! The comments in this file will guide you through the process.
#  Please read all the comments very carefully before changing anything.
#  	The idea of this set of template files (Makefile, Makefile.vars, and
#  all the makevars.mk) is to help organize any C/C++ project with a single
#  Makefile. The advantage is speed. The usual drawback are bad maintainence
#  and modularity. This set of files attempt to cut down all the repetitive
#  work that are usually done manually, i.e., listing target-prerequisites
#  while preserving almost the same degree of modularity as the recursive
#  make approach.
#  TODO:
#  	Write up python or perl scripts that can scan the src/ directory
#  and generate a single Makefile at the top level. The authors has not done
#  this mainly for two reasons: a) He does not know python or perl any well;
#  b) There is still room for improvement in this set of template files.
#  The author wishes to wait until it
#  	a) is tested on several different Linux clusters, and
#  	b) fully supports dynamic _and_ static libraries and executables, and
#  	c) capable of handling primary configure such as DEBUG and RELEASE, and
#  	d) if possible, supports Fortran files well (to appeal to a larger
#  	audience).
#  
#  	The make utility is a good thing. But GNU make lacks in its fundamental
#  design the respect and support for the hierachy of source file directories
#  that are very compilcated for any project with more than a hundred files.
#  Version control and project build are the two most important deeds in 
#  programming, though often their importance is not fully acknowledged. Version
#  control software had Git replaced SVN and CVS. Maybe the call for a better
#  build tool would in time render the long-lived GNU make obselete. People
#  have to come up with ways to do it right. CMake is very good at portability.
#  However, portability also makes CMake less friendly to many of its quasi-
#  expert users, who, just like the author, would like to be able to modify
#  some of the CMake-generated Makefiles.
#  	What it does is to create a building tool based on GNU make and some
#  script files that can do the build in only one pass, and best of all, without
#  introducing any new syntax. Such a simple goal makes the implementation
#  simple and stable. But it only works on Unix-like platforms that has bash
#  shell and python/perl installed. The author would say that this is a, though
#  not big, limitation.
#  	Unlike many other attempts of creating an universally fit building tool
#  for everyone at everywhere at anytime, e.g., CMake, this set of templates,
#  with possible future scripts, aims to do only thing and do it well. The
#  ultimate goal is to realize building with only one pass.
#  	If you have any suggestions and/or ideas, please let me know.
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
${DIR-utils}CFLAGS:=${CFLAGS}
${DIR-utils}CXXFLAGS:=${CXXFLAGS}
${DIR-utils}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE, OBJECT, and LIBRARY FILES
#
#  	The dependencies are automatically resolved by the dependency files (*.d 
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
#  	Only assign the source files here. The corresponding object files are
#  created automatically.
${DIR-utils}CFILES:=file_io.c
${DIR-utils}CPPFILES:=utils.cpp StatVector.cpp Table.cpp TimeStamp.cpp \
	DunavantRule.cpp GaussRule.cpp LynessRule.cpp \
	WandzuraRule.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
#  	Sometimes the name of the dynamic library that is to be build has
#  nothing to do with any of the names of the *.o files. In those cases, we have
#  to specify the entire build rule for the library. For convenience's sake, the
#  complete build rule is copied from the Makefile.vars file.
#${BIN}/%.exe: ${OBJ}/%.o 
	#@echo Linking  "${B_RED}$@${NONE}"...
	#${${DIR-utils}QUIET}${CXX} -o $@ $(filter %.o,$^) $(filter %.so,$^) $(filter %.a,$^) ${LIBS}
#${LIB}/%.so: ${OBJ}/%.o
	#@echo Linking "${B_MAGENTA}$@${NONE}"...
	#${${DIR-utils}QUIET}${CXX} -o $@ $(filter %.o,$^) ${LIBS} ${DYLFLAGS}
${DIR-utils}BINEXE:=msh_to_data.exe
${DIR-utils}DYNLIB:=utils.so QuadratureRules.so

${BIN}/msh_to_data.exe: ${LIB}/utils.so ${OBJ}/msh_to_data.o ${OBJ}/file_io.o 
${LIB}/utils.so: ${OBJ}/utils.o ${OBJ}/StatVector.o ${OBJ}/Table.o \
	${OBJ}/TimeStamp.o
${LIB}/QuadratureRules.so: ${OBJ}/GaussRule.o ${OBJ}/DunavantRule.o \
	${OBJ}/WandzuraRule.o ${OBJ}/LynessRule.o
	@echo Linking "${B_MAGENTA}$@${NONE}"...
	${${DIR-utils}QUIET}${CXX} -o $@ $(filter %.o,$^) ${LIBS} ${DYLFLAGS}
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
#  Speicify all the test files. All test files must be CPP files. But when
#  listing the them in ${DIR-utils}TST, do NOT write the .cpp extension. 
#  For example, if one wishes to add test_mytest.cpp, he should write:
#  		${DIR-utils}TST:=test_mytest
#  Then list all the build rules right afterwards, e.g.:
#		${BUILD}/test_mytest.exe: ${BUILD}/test_mytest.o \
#					  ${BUILD}/any-other-files.o
${DIR-utils}TSTEXE:=test_utils.exe test_QuadratureRules.exe
${BIN}/test_utils.exe: ${LIB}/utils.so  ${OBJ}/test_utils.o 
${BIN}/test_QuadratureRules.exe: ${LIB}/QuadratureRules.so  ${LIB}/utils.so ${OBJ}/test_QuadratureRules.o 
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
${DIR-utils}CFILES:=${${DIR-utils}CFILES:%=${DIR-utils}/%}
${DIR-utils}CPPFILES:=${${DIR-utils}CPPFILES:%=${DIR-utils}/%}
${DIR-utils}OBJFILES:=${${DIR-utils}CPPFILES:${DIR-utils}%.cpp=${OBJ}%.o} \
		      ${${DIR-utils}CFILES:${DIR-utils}%.c=${OBJ}%.o}
${DIR-utils}DEPFILES:=${${DIR-utils}OBJFILES:%.o=%.d}
${DIR-utils}ASMFILES:=${${DIR-utils}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-utils}BINEXE:=${${DIR-utils}BINEXE:%=${BIN}/%}
${DIR-utils}BINCPP:=${${DIR-utils}BINEXE:${BIN}/%.exe=${DIR-utils}/%.cpp}
${DIR-utils}BINOBJ:=${${DIR-utils}BINCPP:${DIR-utils}%.cpp=${OBJ}%.o}
${DIR-utils}BINDEP:=${${DIR-utils}BINOBJ:%.o=%.d}
${DIR-utils}BINASM:=${${DIR-utils}BINOBJ:${OBJ}%.o=${ASM}%.s}
${DIR-utils}DYNLIB:=${${DIR-utils}DYNLIB:%=${LIB}/%}
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
#  C sources
${OBJ}/%.o: ${DIR-utils}/%.c
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
${DIR-utils}-all: ${${DIR-utils}OBJFILES} ${${DIR-utils}BINEXE}	\
	${${DIR-utils}DYNLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-utils}-test: ${${DIR-utils}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-utils}-asm: ${${DIR-utils}ASMFILES} ${${DIR-utils}TSTASM} 	\
	${${DIR-utils}BINASM}
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
		DYNLIB							\
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
