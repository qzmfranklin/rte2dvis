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
#  all the makevars.mk) aim to help organize any C/C++ project with a single
#  Makefile. The advantage is speed. The usual drawback are bad maintainence
#  and modularity. This set of files attempt to cut down all the repetitive
#  work that are usually done manually, i.e., listing target-prerequisites
#  while preserving almost the same degree of modularity as the recursive
#  make approach.
#  	If you have any suggestions and/or ideas, please let me know.
###############################################################################
# Project name, version, and date information.
# ${TARBALL_NAME} is the tarball name when one enters
# 	$ make dist
PROJECT_NAME	:= rte2dvis
VERSION		:= v0.01
DATE_TIME	:= $(shell date "+%Y-%m-%d")
RELEASE_NAME	:= ${PROJECT_NAME}-${VERSION}-${DATE_TIME}
###############################################################################
-include Makefile.vars# The top level vars file
-include ${MKVFILES}#
-include ${DEPFILES}# Dependency files (*.d files)
DISTFILES:=${SRCFILES} ${HDRFILES} ${MANFILES} ${EXPFILES} ${MKVFILES} ${MAKFILES}
###############################################################################
.PHONY: all 							\
	test							\
	asm							\
	list							\
	clean cleanx cleanxx 					\
	dist distclean						\
	check							\
	todolist						\
	mkdir							

all:	${TARGET_ALL} 
test:	${TARGET_TEST} 
asm:	${TARGET_ASM} 
list:	${TARGET_LIST} 
clean:
	@echo "Deleting ${REV_RED}${BUILD}/*${NONE}..."
	@rm -rf ${BUILD}/*
cleanx:
	@echo "Deleting ${REV_GREEN}${OUTPUT}/*${NONE}..."
	@rm -rf ${OUTPUT}/*
cleanxx: clean
	@echo "Deleting ${REV_BLUE}${BIN}/*${NONE}..."
	@rm -rf ${BIN}/*
dist:
	@echo Compressing into "${B_CYAN}${RELEASE_NAME}.tgz${NONE}"...
	@rm -f ${RELEASE_NAME}.tgz; tar czf ${RELEASE_NAME}.tgz ${DISTFILES}
check:
	#TODO
	@echo Checking distribution integrity:
	@echo ${DISTFILES}
todolist: 
	-@for file in ${DISTFILES:=};			\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED		\
	$$file;	done; true 
mkdir: 
	@echo Creating structurual directories:
	@$(foreach dir,${STRUCTURAL_DIRS},if [ ! -d ${dir} ]; 	\
		then echo Creating "${GREEN}${dir}/${NONE}"...; \
		mkdir ${dir}; else echo Already exists:		\
		"${GREY}${dir}${NONE}"; fi;)
rmdir: 
	@echo "${B_RED}WARNING: Deleting ${STRUCTURAL_DIRS}${NONE}"
	@rm -rf ${STRUCTURAL_DIRS}
###############################################################################
