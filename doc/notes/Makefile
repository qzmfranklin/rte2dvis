SRC:=sec1.tex
DVI:=main.dvi $(SRC:%.tex=%.dvi)
PDF:=out.pdf

all: ${PDF}

clean:
	rm -f *.aux *.dvi *.log *.out log
cleanxx: clean
	rm -f ${PDF}

${PDF}: main.dvi ${DVI}
	dvipdf $< $@

main.dvi: main.tex ${SRC}
	latex $< $@

%.dvi: %.tex
	latex $< $@
