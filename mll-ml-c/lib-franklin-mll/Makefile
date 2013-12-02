# automatic function inlining
#NOINLINE := True
# generat universal binary usig -fPIC instead of fpic
UNIVERSAL_BINARY := True
-include ../makevars.mk
INCS	:= ${MLL_INCS} ${ML_INCS}
LIBS	:= ${MLL_LIBS} ${ML_LIBS} ${MKL_LIBS}
############################################################################### 
all: lib-franklin.dylib
install: lib-franklin.dylib
	cp -r $< ../MLL-lib/$<
	make cleanxx 
uninstall:
	rm -rf ../MLL-lib/lib-franklin.dylib
	make clean
############################################################################### 
lib-franklin.dylib: lib-franklin.o legendre-rule.o dunavant-rule.o 

lib-franklin.o: lib-franklin.c
-include lib-franklin.d

legendre-rule.o: legendre-rule.c
-include legendre-rule.d

dunavant-rule.o: dunavant-rule.c
-include dunavant-rule.d
