# Directory name. NO SPACE! NO DEFERRED-EVALUATION!
DIR		:=QuadratureRules
###############################################################################
# List all the directory-specific source files here:
${DIR}CFILES	:=		
${DIR}CPPFILES	:=		${DIR}/DunavantRule.cpp 			\
				${DIR}/WandzuraRule.cpp				\
				${DIR}/GaussQuadratures.cpp
# Specify all the directory-specific binary targets here:
${BUILD}/${DIR}-test.exe: 	${BUILD}/${DIR}-test.o 				\
				${BUILD}/GaussQuadratures.o 			\
				${BUILD}/DunavantRule.o 			\
				${BUILD}/WandzuraRule.o 
###############################################################################
# Directory-specific compiling and linking options
${DIR}CFLAGS 	:=${CFLAGS}
${DIR}CXXFLAGS	:=${CXXFLAGS} \
	-Wno-deprecated# Turn off annoying warning message on deprecated headers
${DIR}INCS	:=${INCS}
${DIR}LIBS	:=${LIBS}
###############################################################################
# Usually, one should NOT need to modify this section.
${DIR}OBJFILES :=${${DIR}CPPFILES:${DIR}%.cpp=${BUILD}%.o}# Object Files
${DIR}DEPFILES :=${${DIR}OBJFILES:%.o=%.d}# Dependency Files in the same directory as .o files
${DIR}ASMFILES :=${${DIR}CPPFILES:${DIR}%.cpp=${DEBUG}%.s}# Commented Assembly Codes
${DIR}TSTFILES :=${DIR}/${DIR}-test.cpp# Testing source files
${DIR}BINFILES :=${${DIR}TSTFILES:%.cpp=%.exe}# All exe files
###############################################################################
# Directory-specific pattern rules
# Usually, one should NOT need to modify this section.
# C++ linkage at the top level
${BUILD}/%.exe: ${BUILD}/%.o 
	@echo Linking executable "${B_RED}$@${NONE}"...
	@${CXX} -o $@ ${filter %.o,$^} ${${DIR}LIBS} 
# C++ sources
${BUILD}/%.o: ${DIR}/%.cpp
	@echo Compiling "${MAGENTA}$@${NONE}"...
	@${CXX} -o $@ -c $< ${${DIR}CXXFLAGS} ${${DIR}INCS}
${DEBUG}/%.s: ${DIR}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	@${CXX} -o $@ $< ${ASMFLAGS} ${${DIR}CXXFLAGS} ${${DIR}INCS} 
# C sources
${BUILD}/%.o: ${DIR}/%.c
	@echo Compiling "${MAGENTA}$@${NONE}"...
	@${CC} -o $@ -c $< ${${DIR}CFLAGS} ${${DIR}INCS}
${DEBUG}/%.s: ${DIR}/%.c
	@echo Generating "${CYAN}$@${NONE}"...
	@${CC} -o $@ $< ${ASMFLAGS} ${${DIR}CFLAGS} ${${DIR}INCS} 
###############################################################################
# Directory-specific PHONY targets
# Usually, one should NOT need to modify this section.
.PHONY: ${DIR}-all ${DIR}-check ${DIR}-test
${DIR}-all: ${${DIR}OBJFILES}
${DIR}-check:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}${DIR}${NONE}"\#\#\#\#\#\#\#\#
	@echo "${BROWN}${DIR}CPPFILES${NONE}"= ${${DIR}CPPFILES}
	@echo "${BROWN}${DIR}OBJFILES${NONE}"= ${${DIR}OBJFILES}
	@echo "${BROWN}${DIR}DEPFILES${NONE}"= ${${DIR}DEPFILES}
	@echo "${BROWN}${DIR}ASMFILES${NONE}"= ${${DIR}ASMFILES}
${DIR}-test: ${BUILD}/${DIR}-test.exe
	@echo Running "${B_BROWN}$<${NONE}"...
	@./$< > ${DEBUG}/${DIR}-test.txt
	@echo Output to "${GREEN}${DEBUG}/${DIR}-test.txt${NONE}".
