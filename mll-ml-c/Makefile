.PHONY:		all				\
       		install uninstall		\
       		clean cleanx cleanxx		\
       		./dunavant-rule-ml/		\
       		./legendre-rule-ml/		\
       		./lib-franklin-mll/

all: 		install

./dunavant-rule-ml/:
	make --directory=$@ install
./legendre-rule-ml/:
	make --directory=$@ install
./lib-franklin-mll/:
	make --directory=$@ install

install:	./dunavant-rule-ml/		\
		./legendre-rule-ml/		\
		./lib-franklin-mll/
uninstall:
	make --directory=./dunavant-rule-ml/ uninstall
	make --directory=./legendre-rule-ml/ uninstall 
	make --directory=./lib-franklin-mll/ uninstall 
clean:  cleanxx
cleanxx:
	make --directory=./dunavant-rule-ml/ cleanxx
	make --directory=./legendre-rule-ml/ cleanxx
	make --directory=./lib-franklin-mll/ cleanxx 
	rm -rf 	./ML-exe/* ./MLL-lib/*
