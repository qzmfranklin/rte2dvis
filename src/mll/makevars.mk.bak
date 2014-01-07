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
#  The ${DIR-mll} variable will be used through the rest of this 
#  makevars.mk file to achieve modularity.
#  	Be very careful:
# 		NO SPACE ( tab is OK )
# 		NO DEFERRED-EVALUATION ( always use colon-equal, i.e., := ) #
DIR-mll:=src/mll# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR-mll}QUIET:=@
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
# 	 	${DIR-mll}CFLAGS :=${CFLAGS} [directory-specific options]
#  	Usually, ${DIR-mll}INCS and should not need to be modified if
#  the build in this directory does not require some extra libraries and/or 
#  hearder files. But in case it did, free at ease to modify these two 
#  variables.
#  	Note that one _cannot_ drop any of the global compiling and linking
#  options. So only specify options that are definitely needed by the entire
#  project at the top level Makefile.
${DIR-mll}CXXFLAGS:=-fPIC -O3
${DIR-mll}INCS:=${MLL_INCS} -iquote include
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR-mll}CPPFILES:=dunavant-rule.cpp legendre-rule.cpp mll.cpp
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR-mll}BINEXE:=
${DIR-mll}LIB:=mll

${LIB}/libmll.so: ${OBJ}/mll.o \
	${OBJ}/dunavant-rule.o ${OBJ}/legendre-rule.o

###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR-mll}TSTEXE:=
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
${DIR-mll}CPPFILES:=${${DIR-mll}CPPFILES:%=${DIR-mll}/%}
${DIR-mll}OBJFILES:=${${DIR-mll}CPPFILES:${DIR-mll}%.cpp=${OBJ}%.o}
${DIR-mll}DEPFILES:=${${DIR-mll}OBJFILES:%.o=%.d}
${DIR-mll}ASMFILES:=${${DIR-mll}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR-mll}BINEXE:=${${DIR-mll}BINEXE:%=${BIN}/%}
${DIR-mll}BINCPP:=${${DIR-mll}BINEXE:${BIN}/%.exe=${DIR-mll}/%.cpp}
${DIR-mll}BINOBJ:=${${DIR-mll}BINCPP:${DIR-mll}%.cpp=${OBJ}%.o}
${DIR-mll}BINDEP:=${${DIR-mll}BINOBJ:%.o=%.d}
${DIR-mll}BINASM:=${${DIR-mll}BINOBJ:${OBJ}%.o=${ASM}%.s}
${DIR-mll}DYNLIB:=${${DIR-mll}LIB:%=${LIB}/lib%.so}
${DIR-mll}STCLIB:=${${DIR-mll}LIB:%=${LIB}/lib%.a}
################## DO NOT MODIFY ################
${DIR-mll}TSTEXE:=${${DIR-mll}TSTEXE:%=${BIN}/%}
${DIR-mll}TSTCPP:=${${DIR-mll}TSTEXE:${BIN}/%.exe=${DIR-mll}/%.cpp}
${DIR-mll}TSTOBJ:=${${DIR-mll}TSTCPP:${DIR-mll}%.cpp=${OBJ}%.o}
${DIR-mll}TSTDEP:=${${DIR-mll}TSTOBJ:%.o=%.d}
${DIR-mll}TSTASM:=${${DIR-mll}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR-mll}CFILES} \
	${${DIR-mll}CPPFILES} ${${DIR-mll}TSTCPP} \
	${${DIR-mll}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR-mll}DEPFILES} \
	${${DIR-mll}TSTDEP} ${${DIR-mll}BINDEP}
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
${OBJ}/%.o: ${DIR-mll}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR-mll}QUIET}${CXX} -o $@ -c $< ${${DIR-mll}CXXFLAGS} ${${DIR-mll}INCS}
${ASM}/%.s: ${DIR-mll}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR-mll}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR-mll}CXXFLAGS} ${${DIR-mll}INCS} 
#DIR-mllECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR-mll}-all ${DIR-mll}-test \
	${DIR-mll}-asm ${DIR-mll}-check \
	${DIR-mll}-list
	
TARGET_ALL	:=${TARGET_ALL} ${DIR-mll}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR-mll}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR-mll}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR-mll}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR-mll}-list
${DIR-mll}-all: ${${DIR-mll}OBJFILES} ${${DIR-mll}BINEXE}	\
	${${DIR-mll}DYNLIB} ${${DIR-mll}STCLIB}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR-mll}-test: ${${DIR-mll}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR-mll}-asm: ${${DIR-mll}ASMFILES} ${${DIR-mll}TSTASM} \
	${${DIR-mll}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR-mll}-check: ${${DIR-mll}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR-mll}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}BEGIN $@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR-mll}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-mll}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR-mll}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-mll}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		DYNLIB STCLIB							\
		,							\
		if [ ! -z "${${DIR-mll}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-mll}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${MAGENTA}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR-mll}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR-mll}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
	@#@$(foreach dir, 						\
		#ASMFILES TSTASM BINASM					\
		#,							\
		#if [ ! -z "${${DIR-mll}${dir}}" ]; then 		\
			#echo "${BROWN}${dir}${NONE}\t\c";		\
			#$(foreach file,${${DIR-mll}${dir}},		\
			    #if [ -f ${file} ]; then echo		\
				#"${CYAN}${file}${NONE}\c";		\
			    #else echo "${GREY}${file}${NONE}\c";	\
			    #fi;)					\
			#echo;						\
		#fi;)
