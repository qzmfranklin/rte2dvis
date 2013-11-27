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
#  The ${DIR-test_mem_bw/mem_bw} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-test_mem_bw/mem_bw:=src/test_mem_bw/mem_bw# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR-test_mem_bw/mem_bw}QUIET:=@
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
# 	 	${DIR-test_mem_bw/mem_bw}CFLAGS :=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-test_mem_bw/mem_bw}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
#  	Note that one _cannot_ drop any of the global compiling and linking
#  options. So only specify options that are definitely needed by the entire
#  project at the top level Makefile.
${DIR-test_mem_bw/mem_bw}CFLAGS:=${CFLAGS}
${DIR-test_mem_bw/mem_bw}CXXFLAGS:=${CXXFLAGS}
${DIR-test_mem_bw/mem_bw}INCS:=${INCS}
###############################################################################
#				STEP 3
#DIRECTORY-SPECIFIC SOURCE, OBJECT, and LIBRARY FILES
#
#  Remember to add the ${DIR-test_mem_bw/mem_bw}/ to whatever source files(s) you wish to 
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
${DIR-test_mem_bw/mem_bw}CFILES:=
${DIR-test_mem_bw/mem_bw}CPPFILES:=${DIR-test_mem_bw/mem_bw}/sumcopy.cpp
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
${DIR-test_mem_bw/mem_bw}LIBFILES:=
###############################################################################
#				STEP 4
#DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
#
#  Executables listed in ${DIR}BINFILES are considered the final output of
#  this project. All of them should be built into ${BIN} instead of ${BUILD}.
#  	Also, specify the dependencies for each binaries.
${DIR-test_mem_bw/mem_bw}BIN:=
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
${DIR-test_mem_bw/mem_bw}BINLIB:=
###############################################################################
#				STEP 5
#DIRECTORY-SPECIFIC TEST FILES
#  Speicify all the test files. All test files must be CPP files. But when
#  listing the them in ${DIR-test_mem_bw/mem_bw}TST, do NOT write the .cpp extension. 
#  For example, if one wishes to add test_mytest.cpp, he should write:
#  		${DIR-test_mem_bw/mem_bw}TST:=test_mytest
#  Then list all the build rules right afterwards, e.g.:
#		${BUILD}/test_mytest.exe: ${BUILD}/test_mytest.o \
#					  ${BUILD}/any-other-files.o
${DIR-test_mem_bw/mem_bw}TST:=test_sumcopy test_mem_bw
${BUILD}/test_sumcopy.exe: ${BIN}/c_utils.dylib ${BIN}/cpp_utils.dylib \
	${BUILD}/sumcopy.o ${BUILD}/test_sumcopy.o
${BUILD}/test_mem_bw.exe: ${BIN}/c_utils.dylib ${BIN}/cpp_utils.dylib \
	${BUILD}/sumcopy.o ${BUILD}/test_mem_bw.o
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
${DIR-test_mem_bw/mem_bw}OBJFILES:=${${DIR-test_mem_bw/mem_bw}CPPFILES:${DIR-test_mem_bw/mem_bw}%.cpp=${BUILD}%.o} \
		      ${${DIR-test_mem_bw/mem_bw}CFILES:${DIR-test_mem_bw/mem_bw}%.c=${BUILD}%.o}
${DIR-test_mem_bw/mem_bw}DEPFILES:=${${DIR-test_mem_bw/mem_bw}OBJFILES:%.o=%.d}
${DIR-test_mem_bw/mem_bw}ASMFILES:=${${DIR-test_mem_bw/mem_bw}OBJFILES:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-test_mem_bw/mem_bw}BINCPP:=${${DIR-test_mem_bw/mem_bw}BIN:%=${DIR-test_mem_bw/mem_bw}/%.cpp}
${DIR-test_mem_bw/mem_bw}BINOBJ:=${${DIR-test_mem_bw/mem_bw}BINCPP:${DIR-test_mem_bw/mem_bw}%.cpp=${BUILD}%.o}
${DIR-test_mem_bw/mem_bw}BINDEP:=${${DIR-test_mem_bw/mem_bw}BINOBJ:%.o=%.d}
${DIR-test_mem_bw/mem_bw}BINEXE:=${${DIR-test_mem_bw/mem_bw}BINOBJ:${BUILD}%.o=${BIN}%.exe}
${DIR-test_mem_bw/mem_bw}BINASM:=${${DIR-test_mem_bw/mem_bw}BINOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-test_mem_bw/mem_bw}TSTCPP:=${${DIR-test_mem_bw/mem_bw}TST:%=${DIR-test_mem_bw/mem_bw}/%.cpp}
${DIR-test_mem_bw/mem_bw}TSTOBJ:=${${DIR-test_mem_bw/mem_bw}TSTCPP:${DIR-test_mem_bw/mem_bw}%.cpp=${BUILD}%.o}
${DIR-test_mem_bw/mem_bw}TSTDEP:=${${DIR-test_mem_bw/mem_bw}TSTOBJ:%.o=%.d}
${DIR-test_mem_bw/mem_bw}TSTEXE:=${${DIR-test_mem_bw/mem_bw}TSTOBJ:%.o=%.exe}
${DIR-test_mem_bw/mem_bw}TSTASM:=${${DIR-test_mem_bw/mem_bw}TSTOBJ:${BUILD}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR-test_mem_bw/mem_bw}CFILES} \
	${${DIR-test_mem_bw/mem_bw}CPPFILES} ${${DIR-test_mem_bw/mem_bw}TSTCPP} \
	${${DIR-test_mem_bw/mem_bw}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR-test_mem_bw/mem_bw}DEPFILES} \
	${${DIR-test_mem_bw/mem_bw}TSTDEP} ${${DIR-test_mem_bw/mem_bw}BINDEP}
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
${BUILD}/%.o: ${DIR-test_mem_bw/mem_bw}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-test_mem_bw/mem_bw}QUIET}${CXX} -o $@ -c $< ${${DIR-test_mem_bw/mem_bw}CXXFLAGS} ${${DIR-test_mem_bw/mem_bw}INCS}
${ASM}/%.s: ${DIR-test_mem_bw/mem_bw}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-test_mem_bw/mem_bw}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-test_mem_bw/mem_bw}CXXFLAGS} ${${DIR-test_mem_bw/mem_bw}INCS} 
#  C sources
${BUILD}/%.o: ${DIR-test_mem_bw/mem_bw}/%.c
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-test_mem_bw/mem_bw}QUIET}${CC} -o $@ -c $< ${${DIR-test_mem_bw/mem_bw}CFLAGS} ${${DIR-test_mem_bw/mem_bw}INCS}
${ASM}/%.s: ${DIR-test_mem_bw/mem_bw}/%.c
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-test_mem_bw/mem_bw}QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR-test_mem_bw/mem_bw}CFLAGS} ${${DIR-test_mem_bw/mem_bw}INCS} 
#DIR-test_mem_bw/mem_bwECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR-test_mem_bw/mem_bw}-all ${DIR-test_mem_bw/mem_bw}-test ${DIR-test_mem_bw/mem_bw}-asm ${DIR-test_mem_bw/mem_bw}-list
TARGET_ALL	:=${TARGET_ALL} ${DIR-test_mem_bw/mem_bw}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-test_mem_bw/mem_bw}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-test_mem_bw/mem_bw}-asm
TARGET_LIST	:=${TARGET_LIST} ${DIR-test_mem_bw/mem_bw}-list
${DIR-test_mem_bw/mem_bw}-all: ${${DIR-test_mem_bw/mem_bw}OBJFILES} ${${DIR-test_mem_bw/mem_bw}BINEXE}		\
	${${DIR-test_mem_bw/mem_bw}LIBFILES} ${${DIR-test_mem_bw/mem_bw}BINLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-test_mem_bw/mem_bw}-test: ${${DIR-test_mem_bw/mem_bw}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-test_mem_bw/mem_bw}-asm: ${${DIR-test_mem_bw/mem_bw}ASMFILES} ${${DIR-test_mem_bw/mem_bw}TSTASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-test_mem_bw/mem_bw}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR-test_mem_bw/mem_bw}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-test_mem_bw/mem_bw}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR-test_mem_bw/mem_bw}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-test_mem_bw/mem_bw}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		LIBFILES BINLIB						\
		,							\
		if [ ! -z "${${DIR-test_mem_bw/mem_bw}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-test_mem_bw/mem_bw}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${MAGENTA}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR-test_mem_bw/mem_bw}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-test_mem_bw/mem_bw}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		ASMFILES TSTASM BINASM					\
		,							\
		if [ ! -z "${${DIR-test_mem_bw/mem_bw}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-test_mem_bw/mem_bw}${dir}},		\
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
