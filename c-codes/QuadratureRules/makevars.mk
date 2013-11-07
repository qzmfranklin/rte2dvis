###############################################################################
DIR	:=QuadratureRules # No space! Crucial!
###############################################################################
# Directory-specific files
${DIR}MAKFILES:= ${MAKFILES} ${DIR}/makevars.mk # Makfile and Related File(s)
${DIR}TSTFILES:= ${TSTFILES} ${DIR}/test.cpp
${DIR}HDRFILES:= ${HDRFILES} ${DIR}
${DIR}CPPFILES:= ${CPPFILES} 	${DIR}/DunavantRule.cpp ${DIR}/GaussQuadratures.cpp \
				${DIR}/WandzuraRule.cpp
#${DIR}CPPFILES:= ${CPPFILES} $(wildcard ${DIR}/*.cpp)
${DIR}DEPFILES:= ${DEPFILES} ${OBJFILES:%.o=%.d} # Dependency Files
${DIR}OBJFILES:= ${CFILES:%.c=%.o} ${CPPFILES:%.cpp=%.o} # Object Files
${DIR}ASMFILES:= ${OBJFILES:%.o=%.s} # Commented Assembly Codes
${DIR}BINFILES:= ${TSTFILES:%.cpp=%.exe} ./file-io/mshtodat.exe # All EXE Files
${DIR}AUXFILES:= ${DOCFILES} ${EXPFILES} ${MANFILES} ${MAKFILES} # Auxilliary Files
${DIR}ALLFILES:= ${SRCFILES} ${HDRFILES} ${AUXFILES} # All Files for Distribution
###############################################################################
# Directory-specific compiler options
${DIR}CFLAGS	:= ${CFLAGS} 
${DIR}CXXFLAGS:= ${CXXFLAGS}
###############################################################################
# Directory-specific pattern rules
%.o: %.cpp
	@echo Compiling "${B_RED}$@${NONE}"...
	@${CXX} -o $@ -c $< -MD -MP ${CXXFLAGS} ${INCS}
%.s: %.cpp
	@echo Generating source-commented assembly list "${BROWN}$@${NONE}"...
	@${CXX} -o $@ -S $< ${CXXFLAGS} -fsource-asm ${INCS} 
###############################################################################
