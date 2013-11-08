###############################################################################
# Project name, version, and date information.
# ${TARBALL_NAME} is the tarball name when one enters
# 	$ make dist
PROJECT_NAME	:= rte2dvis
VERSION		:= v0.01
DATE_TIME	:= $(shell date "+%Y-%m-%d")
TARBALL_NAME	:= ${PROJECT_NAME}-${VERSION}-${DATE_TIME}.tgz
###############################################################################
-include makevars.mk
-include src/QuadratureRules/makevars.mk
-include ${DEPFILES} # Dependency files (*.d files)
###############################################################################
.SUFFIXES:
.SUFFIXES: .tm .c .cpp .o .exe .s .d 
.PHONY: all 						\
	clean cleanx cleanxx 				\
	dist						\
	distclean					\
	check						\
	test						\
	install uninstall				\
	commit						\
	todolist

all: ${OBJFILES}

clean:
	@echo "${REV_BLUE}Deleting testing outputs...${NONE}"
	@rm -rf ${DEBUG}/*
cleanx: 
	@echo "${REV_GREEN}Deleting commented assembly codes...${NONE}"
	@rm -rf ${DEBUG}/*
cleanxx: 
	@echo "${REV_RED}Deleting everything but the sources...${NONE}"
	@rm -rf ${BUILD}/*

dist:
	@echo Compressing into "${B_CYAN}${TARBALL_NAME}${NONE}"...
	@tar czf ${TARBALL_NAME} ${ALLFILES}
distclean:
	@rm -rf *.tgz
check:
	@echo "${B_BROWN}INCLUDES${NONE}" = ${INCLUDES}
	@echo "${B_BROWN}TSTFILES${NONE}" = ${TSTFILES}
	@echo "${B_BROWN}SRCFILES${NONE}" = ${SRCFILES}
	@echo "${B_BROWN}OBJFILES${NONE}" = ${OBJFILES}
	@echo "${B_BROWN}DEPFILES${NONE}" = ${DEPFILES}
	@echo "${B_BROWN}BINFILES${NONE}" = ${BINFILES}
	@echo "${B_BROWN}ALLFILES${NONE}" = ${ALLFILES}
test: ${TSTFILES:%.cpp=%.exe} ./file-io/mshtodat.exe
	@echo Running "${B_BROWN}./file-io/mshtodat.exe${NONE}"...
	@./file-io/mshtodat.exe		./file-io/example.msh			\
					DEBUG/test-mshtodat			\
					> DEBUG/test-mshtodat.txt
	@echo Running "${B_BROWN}./file-io/test.exe${NONE}"...
	@./file-io/test.exe 		DEBUG/arg1 DEBUG/helloworld.txt 	\
					./file-io/example.msh 			\
					DEBUG/test-file-io-example		\
					> DEBUG/test-file-io.txt
	@echo Running "${B_BROWN}./QuadratureRules/test.exe${NONE}"...
	@./QuadratureRules/test.exe 	> DEBUG/test-QuadratureRules.txt
	@echo Running "${B_BROWN}./utils/test.exe${NONE}"...
	@./utils/test.exe		> DEBUG/test-utils.txt
	@echo Testing information is in "${B_GREEN}DEBUG/${NONE}".
install: all

commit:
	@echo Git committing...
	@cd ../; \
		open .gitignore; \
		git add --all; 	\
		git commit -m "$(shell date "+%Y%m%d %H:%M:%S")"; \
		cd - 

todolist: 
	-@for file in ${ALLFILES:Makefile=};		\
	do fgrep -H -e TODO -e FIXME -e UNFINISHED	\
	$$file;	done; true 
###############################################################################
