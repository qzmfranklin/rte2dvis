
all: libmll.so

libmll.so: mll.o dunavant-rule.o legendre-rule.o
	icpc -o $@ $^ -shared

%.o:%.cpp
	icpc -c $< -fPIC \
		-I/opt/Wolfram/Mathematica/9.0/SystemFiles/IncludeFiles/C \
		-iquote include

clean:
	rm *.o
cleanxx:
	rm *.o libmll.so
