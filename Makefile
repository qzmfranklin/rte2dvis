.PHONY:		all			\
		install uninstall	\
		clean cleanx cleanxx	\
		./mll-ml-c/

all:		install

./mll-ml-c/:
	make --directory=$@ install
install:	./mll-ml-c/
	ln -sf $<ML-exe ./ML-exe
	ln -sf $<MLL-lib ./MLL-lib
uninstall:
	make --directory=./mll-ml-c/ uninstall
	rm -rf ML-exe 
	rm -rf MLL-lib
clean:  cleanxx
cleanxx:
	make --directory=./mll-ml-c/ cleanxx

