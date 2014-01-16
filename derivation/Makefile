SRC:=main.tex sec1.tex
DVI:=$(SRC:%.tex=%.dvi)
PDF:=derivation.pdf

all: ${PDF}

clean:
	rm -f *.aux *.dvi *.log *.out log
cleanxx: clean
	rm -f *.pdf

${PDF}: ${DVI}
	dvipdf $< $@
%.dvi: %.tex
	latex $< $@
	latex $< $@
	latex $< $@
