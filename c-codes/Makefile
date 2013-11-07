###############################################################################
# Project name, version, and date information.
# ${TARBALL_NAME} is the tarball name when one enters
# 	$ make dist
PROJECT_NAME	:= rte2dvis
VERSION		:= v0.01
DATE_TIME	:= $(shell date "+%Y-%m-%d")
TARBALL_NAME	:= ${PROJECT_NAME}-${VERSION}-${DATE_TIME}.tgz
###############################################################################
-include Makefile.var
###############################################################################
# Dependencies for executables
./file-io/mshtodat.exe:		./file-io/mshtodat.o 			\
				./file-io/file-io.o 
./file-io/test.exe: 		./file-io/test.o			\
				./file-io/file-io.o 
./QuadratureRules/test.exe:	./QuadratureRules/test.o		\
				./QuadratureRules/DunavantRule.o	\
				./QuadratureRules/WandzuraRule.o	\
				./QuadratureRules/GaussQuadratures.o 
./utils/test.exe: 		./utils/test.o				\
				./utils/utils.o
