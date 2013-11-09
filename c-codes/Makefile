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
DISTFILES:=${SRCFILES} ${HDRFILES} ${MANFILES} ${EXPFILES} ${MKVFILES} Makefile
###############################################################################
.PHONY: all 						\
	test						\
	asm						\
	list						\
	clean cleanx cleanxx 				\
	dist distclean					\
	check						\
	todolist 

all:	${TARGET_ALL} 
test:	${TARGET_TEST} 
asm:	${TARGET_ASM} 
list:	${TARGET_LIST} 
clean:
	@echo "${REV_RED}Deleting ${BUILD}/*...${NONE}"
	@rm -rf ${BUILD}/*
cleanx:
	@echo "${REV_GREEN}Deleting ${DEBUG}/*...${NONE}"
	@rm -rf ${DEBUG}/*
cleanxx:
	@echo "${REV_GREY}Deleting ${TARBALL_NAME}...${NONE}"
	@rm -rf ${RELEASE_NAME}.tgz
dist:
	@echo Compressing into "${B_CYAN}${RELEASE_NAME}.*${NONE}"...
	@tar czf ${RELEASE_NAME}.*
distclean:
	#TODO
check:
	#TODO
	@echo Checking integrity...
	@echo ${DISTFILES}
todolist: 
	-@for file in ${ALLFILES:Makefile=};		\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED	\
	$$file;	done; true 
###############################################################################
