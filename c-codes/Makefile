###############################################################################
PROJECT_NAME	:= rte2dvis
VERSION		:= v0.01
DATE_TIME	:= $(shell date "+%Y-%m-%d")
TARBALL_NAME	:= ${PROJECT_NAME}-${VERSION}-${DATE_TIME}.tgz
###############################################################################
-include makevars.mk
###############################################################################
-include ${DEPFILES}
###############################################################################
.PHONY: all 						\
	clean cleanx cleanxx 				\
	dist						\
	distclean					\
	check						\
	test						\
	install uninstall				\
	todolist

all: ${OBJFILES}

clean:
	@rm -rf ${ASMFILES} 
cleanx: 
	@rm -rf *.s DEBUG/* OUPUT/*
cleanxx: clean
	@rm -rf ${OBJFILES} ${DEPFILES} ${BINFILES}
dist:
	@echo Compressing into "${CYAN}${TARBALL_NAME}${NONE}"...
	@tar czf ${TARBALL_NAME} ${ALLFILES}
distclean:
	@rm -rf *.tgz
test: ${TSTFILES:%.cpp=%.exe}
	@echo Running "${RED}$<${NONE}"...
	@./$< > DEBUG/test.txt
	@echo Debug information is in "${GREEN}DEBUG/test.txt${NONE}".
check:
	@echo "${YELLOW}INCLUDES${NONE}" = ${INCLUDES}
	@echo "${YELLOW}TSTFILES${NONE}" = ${TSTFILES}
	@echo "${YELLOW}SRCFILES${NONE}" = ${SRCFILES}
	@echo "${YELLOW}OBJFILES${NONE}" = ${OBJFILES}
	@echo "${YELLOW}DEPFILES${NONE}" = ${DEPFILES}
install: all
	mv ${OBJFILES} ${BIN}
todolist:
	-@for file in ${ALLFILES:Makefile=};		\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED	\
	$$file;	done; true 
###############################################################################
