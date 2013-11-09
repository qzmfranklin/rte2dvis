###############################################################################
# Project name, version, and date information.
# ${TARBALL_NAME} is the tarball name when one enters
# 	$ make dist
PROJECT_NAME	:= rte2dvis
VERSION		:= v0.01
DATE_TIME	:= $(shell date "+%Y-%m-%d")
TARBALL_NAME	:= ${PROJECT_NAME}-${VERSION}-${DATE_TIME}.tgz
###############################################################################
-include Makefile.vars# The top level vars file
-include ${MKVFILES}#
-include ${DEPFILES}# Dependency files (*.d files)
DISTFILES:=${SRCFILES} ${HDRFILES} ${MANFILES} ${EXPFILES} ${MKVFILES} ./Makefile
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
	@rm -f ${BUILD}/*
cleanx:
	@echo "${REV_GREEN}Deleting ${DEBUG}/*...${NONE}"
	@rm -f ${DEBUG}/*
cleanxx:
	@echo "${REV_GREY}Deleting ${TARBALL_NAME}...${NONE}"
	@rm -f ${TARBALL_NAME}
dist:
	@echo Compressing into "${B_CYAN}${TARBALL_NAME}${NONE}"...
	@tar czf ${TARBALL_NAME} ${DISTFILES}
distclean:
	#TODO
check:
	#TODO
	@echo Checking integrity...
todolist: 
	-@for file in ${ALLFILES:Makefile=};		\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED	\
	$$file;	done; true 
###############################################################################
