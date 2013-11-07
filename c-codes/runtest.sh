#!/bin/sh

./file-io/test.exe 	DEBUG/arg1 DEBUG/helloworld.txt 	\
			./file-io/example.msh 			\
			DEBUG/test-file-io-example		\
			> DEBUG/test-file-io.txt

./QuadratureRules/test.exe 	> DEBUG/test-QuadratureRules.txt

./utils/test.exe		> DEBUG/test-utils.txt

