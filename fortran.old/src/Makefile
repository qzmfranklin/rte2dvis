F=ifort
#F=gfortran
Fflag=-O5
incdir=.
libdir=.

#mylib=librte2dsol.dylib
#mylib_src=global.f90 cmd.f90 inputs.f90 zdotx2.f90


#.PHONY: clean cleanall
#main: main.f90 ${mylib}
	#${F} ${Fflag} -o main main.f90 -L${libdir} -I${incdir} -lrte2dsol
#${mylib}:
	#${F} ${Fflag} -dynamiclib -o ${mylib} -L${libdir} -I${incdir} -fpic ${mylib_src}

#clean:
	#rm -rf *.mod *.o
#cleanall: clean
	#rm -rf main ${mylib}

src=global.f90 cmd.f90 inputs.f90 zdotx2.f90
obj=${src:.f90=.o}
exe=main

.PHONY: clean cleanall

${exe}: main.o ${obj}
	${F} ${Fflag} -L${libdir} -I${incdir} -o $@ $^
main.o: main.f90 ${obj}
	${F} ${Fflag} -L${libdir} -I${incdir} -c $^
global.o: global.f90
	${F} ${Fflag} -L${libdir} -I${incdir} -c $^
cmd.o: cmd.f90 global.o
	${F} ${Fflag} -L${libdir} -I${incdir} -c $^
inputs.o: inputs.f90 global.o
	${F} ${Fflag} -L${libdir} -I${incdir} -c $^
zdotx2.o: zdotx2.f90 global.o inputs.o
	${F} ${Fflag} -L${libdir} -I${incdir} -c $^

clean:
	rm -rf *.o *.mod
cleanall: clean
	rm -rf ${exe}
