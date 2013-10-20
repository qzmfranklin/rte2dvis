.PHONY: all clean cleanxx

all: derivation.pdf notes1.pdf notes2.pdf
%.pdf: %.dvi
	dvipdf $^
#notes1.pdf: notes1.dvi
	#dvipdf $^
#%.dvi: %.tex
	#latex $^
	#latex $^
	#latex $^
#derivation.pdf: derivation.dvi
	#dvipdf $^
#notes1.pdf: notes1.dvi
	#dvipdf $^
derivation.dvi: derivation.tex
	latex $^
	latex $^
	latex $^
notes1.dvi: notes1.tex
	latex $^
notes2.dvi: notes2.tex
	latex $^
clean:
	rm -rf *.aux *.bbl *.blg *.log *.synctex *.tex.bak *.dvi *.out *.toc
cleanxx: clean
	rm -rf *.pdf
