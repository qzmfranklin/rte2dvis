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
#  Welcome! The comments in this file will guide you through the process.
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
#  	This method is not portable yet. Though very easily transpotable.
#  Use of BASH shell is strongly recommended. Use of C-shell or other shells
#  might lead to unexpected results.
#  	If you have any suggestions and/or ideas, please let me know.
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
###############################################################################
#				STEP 1
#DIRECTORY NAME FROM THE ROOT DIRECTORY
#
#  The ${DIR-quad} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-quad:=src/quad# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR-quad}QUIET:=@
###############################################################################
#				STEP 2
#DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
#
#  Options specified here are used in this directory. By default, all builds 
#  use the same options. When different compiling and/or linking options 
#  need to be assigned to different targets, the programmer needs to list all
#  the speical target-prerequisite dependencies manually.
#  	By default, the local options inherits the corresponding global ones 
#  from the Makefile in the root directory. If one wishes to add any 
#  additional options that are specific to this very directory, add them after 
#  the global ones. For example:
# 	 	${DIR-quad}CFLAGS :=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-quad}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
#  	Note that one _cannot_ drop any of the global compiling and linking
#  options. So only specify options that are definitely needed by the entire
#  project at the top level Makefile.
${DIR-quad}CFLAGS:=${CFLAGS}
${DIR-quad}CXXFLAGS:=${CXXFLAGS}
${DIR-quad}INCS:=${INCS}
###############################################################################
#				STEP 3
#DIRECTORY-SPECIFIC SOURCE, OBJECT, and LIBRARY FILES
#
#  Remember to add the ${DIR-quad}/ to whatever source files(s) you wish to 
#  add.
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
${DIR-quad}CFILES:=
${DIR-quad}CPPFILES:=${DIR-quad}/DunavantRule.cpp \
	${DIR-quad}/GaussRule.cpp ${DIR-quad}/LynessRule.cpp \
	${DIR-quad}/WandzuraRule.cpp
#  Specify the libraries one wishes to build, and the build rules for them.
#  Note that these libraries are cleaned when one types "make clean".
#  	Note that the default pattern rules for building libraries are %.dylib:
#  %.o based. There must be one and only one .o file that shares the file base
#  name with the library. For example: 
#  		mylib.dylib: my.o			is not valid;
#  		mylib.dylib: mylib.o mylib2.o		is valid;
#  	Also note that the MacOSX system does NOT support statically linked
#  binaries. There is no use building any static library therefore. However,
#  the pattern rules for building static libraries and linking against static
#  libraries are still preserved in the top level Makefile.vars for use in a 
#  Linux system.
${DIR-quad}LIBFILES:=
###############################################################################
#				STEP 4
#DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
#
#  Executables listed in ${DIR}BINFILES are considered the final output of
#  this project. All of them should be built into ${BIN} instead of ${BUILD}.
#  	Also, specify the dependencies for each binaries.
${DIR-quad}BIN:=
#  Specify the libraries one wishes to build, and the build rules for them.
#  Note that these libraries are NOT cleaned when one types "make clean". These
#  libraries are considered parts of the final output of this project.
#  	Note that the default pattern rules for building libraries are %.dylib:
#  %.o based. There must be one and only one .o file that shares the file base
#  name with the library. For example: 
#  		mylib.dylib: my.o			is _not_ valid;
#  		mylib.dylib: mylib.o mylib2.o		is valid.
#  	Also note that the MacOSX system does NOT support statically linked
#  binaries. There is no use building any static library therefore. However,
#  the pattern rules for building static libraries and linking against static
#  libraries are still preserved in the top level Makefile.vars for use in a 
#  Linux system.
${DIR-quad}BINLIB:=
###############################################################################
#				STEP 5
#DIRECTORY-SPECIFIC TEST FILES
#  Speicify all the test files. All test files must be CPP files. But when
#  listing the them in ${DIR-quad}TST, do NOT write the .cpp extension. 
#  For example, if one wishes to add test_mytest.cpp, he should write:
#  		${DIR-quad}TST:=test_mytest
#  Then list all the build rules right afterwards, e.g.:
#		${BUILD}/test_mytest.exe: ${BUILD}/test_mytest.o \
#					  ${BUILD}/any-other-files.o
${DIR-quad}TST:=test_quad_rules
${BUILD}/test_quad_rules.exe: ${BIN}/cpp_utils.dylib ${BIN}/c_utils.dylib \
	${BUILD}/test_quad_rules.o ${BUILD}/DunavantRule.o \
	${BUILD}/GaussRule.o ${BUILD}/LynessRule.o \
	${BUILD}/WandzuraRule.o
###############################################################################
#				STEP 6
#WHATEVER IS LEFT SHALL BE WRITTEN HERE



















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
#	The following four blocks of codes generate proper directory-specific
#  variables and add proper files to the top level variables. Please anyway do
#  NOT modify them.
################## DO NOT MODIFY ################
${DIR-quad}OBJFILES:=${${DIR-quad}CPPFILES:${DIR-quad}%.cpp=${BUILD}%.o} \
		      ${${DIR-quad}CFILES:${DIR-quad}%.c=${BUILD}%.o}
${DIR-quad}DEPFILES:=${${DIR-quad}OBJFILES:%.o=%.d}
${DIR-quad}ASMFILES:=${${DIR-quad}OBJFILES:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-quad}BINCPP:=${${DIR-quad}BIN:%=${DIR-quad}/%.cpp}
${DIR-quad}BINOBJ:=${${DIR-quad}BINCPP:${DIR-quad}%.cpp=${BUILD}%.o}
${DIR-quad}BINDEP:=${${DIR-quad}BINOBJ:%.o=%.d}
${DIR-quad}BINEXE:=${${DIR-quad}BINOBJ:${BUILD}%.o=${BIN}%.exe}
${DIR-quad}BINASM:=${${DIR-quad}BINOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-quad}TSTCPP:=${${DIR-quad}TST:%=${DIR-quad}/%.cpp}
${DIR-quad}TSTOBJ:=${${DIR-quad}TSTCPP:${DIR-quad}%.cpp=${BUILD}%.o}
${DIR-quad}TSTDEP:=${${DIR-quad}TSTOBJ:%.o=%.d}
${DIR-quad}TSTEXE:=${${DIR-quad}TSTOBJ:%.o=%.exe}
${DIR-quad}TSTASM:=${${DIR-quad}TSTOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR-quad}CFILES} \
	${${DIR-quad}CPPFILES} ${${DIR-quad}TSTCPP} \
	${${DIR-quad}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR-quad}DEPFILES} \
	${${DIR-quad}TSTDEP} ${${DIR-quad}BINDEP}
################## DO NOT MODIFY ################
###############################################################################
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
${BUILD}/%.o: ${DIR-quad}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-quad}QUIET}${CXX} -o $@ -c $< ${${DIR-quad}CXXFLAGS} ${${DIR-quad}INCS}
${ASM}/%.s: ${DIR-quad}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-quad}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-quad}CXXFLAGS} ${${DIR-quad}INCS} 
#  C sources
${BUILD}/%.o: ${DIR-quad}/%.c
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-quad}QUIET}${CC} -o $@ -c $< ${${DIR-quad}CFLAGS} ${${DIR-quad}INCS}
${ASM}/%.s: ${DIR-quad}/%.c
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-quad}QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR-quad}CFLAGS} ${${DIR-quad}INCS} 
#DIR-quadECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR-quad}-all ${DIR-quad}-test ${DIR-quad}-asm ${DIR-quad}-list
TARGET_ALL	:=${TARGET_ALL} ${DIR-quad}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-quad}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-quad}-asm
TARGET_LIST	:=${TARGET_LIST} ${DIR-quad}-list
${DIR-quad}-all: ${${DIR-quad}OBJFILES} ${${DIR-quad}BINEXE}		\
	${${DIR-quad}LIBFILES} ${${DIR-quad}BINLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-quad}-test: ${${DIR-quad}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-quad}-asm: ${${DIR-quad}ASMFILES} ${${DIR-quad}TSTASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-quad}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR-quad}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-quad}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR-quad}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-quad}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		LIBFILES BINLIB						\
		,							\
		if [ ! -z "${${DIR-quad}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-quad}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${MAGENTA}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR-quad}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-quad}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		ASMFILES TSTASM BINASM					\
		,							\
		if [ ! -z "${${DIR-quad}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-quad}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${CYAN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@#echo Have not decided the final order of files.		\
		OBJFILES TSTOBJ BINOBJ					\
		DEPFILES TSTDEP BINDEP					\
		ASMFILES TSTASM BINASM					\
		OBJFILES DEPFILES ASMFILES				\
		TSTOBJ TSTASM						\
		BINOBJ BINASM						\
